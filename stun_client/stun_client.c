#include <arpa/inet.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <unistd.h>

#define STUN_PORT       3478         // STUN 标准端口
#define MAGIC_COOKIE    0x2112A442   // STUN 协议魔法数
#define BINDING_REQUEST 0x0001       // 消息类型：Binding Request

// 20 字节 STUN 头部
struct stun_header
{
    uint16_t type;
    uint16_t length;
    uint32_t magic_cookie;
    uint8_t  transaction_id[12];
};

// STUN 属性头部（类型+长度）
struct stun_attr
{
    uint16_t type;
    uint16_t length;
};

int main(int argc, char *argv[])
{
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <STUN服务器地址>\n", argv[0]);
        return 1;
    }

    const char *server = argv[1];
    int         port   = STUN_PORT;

    // 1. 解析服务器地址
    struct hostent *he = gethostbyname(server);
    if (!he) {
        perror("gethostbyname");
        return 1;
    }

    struct sockaddr_in stun_addr;
    memset(&stun_addr, 0, sizeof(stun_addr));
    stun_addr.sin_family = AF_INET;
    stun_addr.sin_port   = htons(port);
    memcpy(&stun_addr.sin_addr, he->h_addr_list[0], he->h_length);

    // 2. 创建 UDP 套接字
    int sock = socket(AF_INET, SOCK_DGRAM, 0);
    if (sock < 0) {
        perror("socket");
        return 1;
    }

    // 3. 构造 Binding Request
    struct stun_header request;
    request.type         = htons(BINDING_REQUEST);
    request.length       = htons(0);   // 没有属性
    request.magic_cookie = htonl(MAGIC_COOKIE);
    // 随机生成事务 ID（这里简单用 0 填充，生产环境应用随机数）
    memset(request.transaction_id, 0, sizeof(request.transaction_id));

    // 4. 发送请求
    ssize_t sent = sendto(
        sock, &request, sizeof(request), 0, (struct sockaddr *)&stun_addr, sizeof(stun_addr));
    if (sent < 0) {
        perror("sendto");
        close(sock);
        return 1;
    }

    printf("Binding Request 已发送到 %s:%d\n", server, port);

    // 5. 接收响应（设置超时 3 秒）
    struct timeval tv;
    tv.tv_sec  = 3;
    tv.tv_usec = 0;
    setsockopt(sock, SOL_SOCKET, SO_RCVTIMEO, &tv, sizeof(tv));

    uint8_t            buffer[1500];
    struct sockaddr_in reply_addr;
    socklen_t          addr_len = sizeof(reply_addr);

    ssize_t recv_len =
        recvfrom(sock, buffer, sizeof(buffer), 0, (struct sockaddr *)&reply_addr, &addr_len);
    if (recv_len < 0) {
        perror("recvfrom");
        close(sock);
        return 1;
    }

    // 6. 检查是否收到合法的 STUN 响应
    if (recv_len < (ssize_t)sizeof(struct stun_header)) {
        fprintf(stderr, "收到的包太短，不是合法的 STUN 响应\n");
        close(sock);
        return 1;
    }

    struct stun_header *response = (struct stun_header *)buffer;
    if (ntohl(response->magic_cookie) != MAGIC_COOKIE) {
        fprintf(stderr, "魔法数不对，不是 STUN 响应\n");
        close(sock);
        return 1;
    }

    // 消息类型高 2 位表示消息类别，0b00 是请求，0b01 是成功响应，0b11 是错误
    uint16_t msg_type = ntohs(response->type);
    if ((msg_type & 0x0110) != 0x0100) {
        fprintf(stderr, "不是 Binding Success 响应 (类型 = 0x%04x)\n", msg_type);
        close(sock);
        return 1;
    }

    // 7. 解析属性，寻找 XOR-MAPPED-ADDRESS (0x0020)
    uint16_t msg_length = ntohs(response->length);
    uint8_t *attr_start = buffer + sizeof(struct stun_header);
    uint8_t *attr_end   = attr_start + msg_length;

    while (attr_start + sizeof(struct stun_attr) <= attr_end) {
        struct stun_attr *attr = (struct stun_attr *)attr_start;
        uint16_t          type = ntohs(attr->type);
        uint16_t          len  = ntohs(attr->length);

        // XOR-MAPPED-ADDRESS 属性类型
        if (type == 0x0020 && len >= 8) {
            uint8_t family = attr_start[4 + 1];   // 第 5 字节：地址族，0x01 = IPv4
            if (family != 0x01) {
                fprintf(stderr, "不支持的地址族: %d\n", family);
                close(sock);
                return 1;
            }

            // 端口在第 6-7 字节（需异或 magic cookie 高 16 位）
            uint16_t xor_port = ntohs(*(uint16_t *)(attr_start + 4 + 2));
            uint16_t port     = xor_port ^ (MAGIC_COOKIE >> 16);

            // IPv4 地址在第 8-11 字节（需异或 magic cookie）
            uint32_t xor_addr = ntohl(*(uint32_t *)(attr_start + 4 + 4));
            uint32_t ip       = xor_addr ^ MAGIC_COOKIE;

            struct in_addr addr;
            addr.s_addr = htonl(ip);

            printf("公网映射地址: %s:%d\n", inet_ntoa(addr), port);
            close(sock);
            return 0;
        }

        // 移动到下一个属性（按 4 字节对齐）
        size_t padded_len = (len + 3) & ~3;
        attr_start += sizeof(struct stun_attr) + padded_len;
    }

    fprintf(stderr, "响应中没有 XOR-MAPPED-ADDRESS 属性\n");
    close(sock);
    return 1;
}
