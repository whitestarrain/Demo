package main

import (
	"bufio"
	"bytes"
	"fmt"
	"image"
	"image/color"
	"image/gif"
	"io"
	"math"
	"math/rand"
	"net/http"
	"os"
	"strconv"
	"strings"
	"time"
	"unicode/utf8"
)

const (
	_   = 1 << (10 * iota)
	KiB // 1024
	MiB // 1048576
	GiB // 1073741824
	TiB // 1099511627776             (exceeds 1 << 32)
	PiB // 1125899906842624
	EiB // 1152921504606846976
	ZiB // 1180591620717411303424    (exceeds 1 << 64)
	YiB // 1208925819614629174706176
)

func main() {
	// str()
	// loop()
	// echo()
	// dup()
	// fetchAll()
	// PopCount(1000)
	// svg(os.Stdout)
	// server()
	// unicodeTest()
	// arr()
	mapTest()
}

func str() {
	fmt.Println("Hello, World!")
	fmt.Println("string slice"[2:5])
	fmt.Println("string slice"[2:])
	fmt.Println(os.Args[0])
	fmt.Println(len(os.Args[0]))
	fmt.Println(strings.Join(os.Args[1:], " "))
}

func loop() {
	for i := 1; i < 5; i++ {
		fmt.Println(i)
	}
	for {
		fmt.Println("temp")
		break
	}
}

func echo() {
	var s, sep string
	for i := 1; i < len(os.Args); i++ {
		s += sep + os.Args[i]
		sep = " "
	}
	for index, arg := range os.Args {
		fmt.Println(index, arg)
	}
	fmt.Println(s)
}

func dup() {
	countLines := func(f *os.File, counts map[string]int) {
		input := bufio.NewScanner(f)
		for input.Scan() {
			counts[input.Text()]++
		}
		for line, n := range counts {
			fmt.Printf("%d\t%s\n", n, line)
		}
	}

	counts := make(map[string]int)
	// input := bufio.NewScanner(os.Stdin)
	file, err := os.Open(os.Args[1])
	if err != nil {
		fmt.Fprintf(os.Stderr, "err is %v\n", err)
		return
	}
	countLines(file, counts)
	// fmt.Println(counts)
}

func lissajous(out io.Writer) {
	var palette = []color.Color{color.White, color.Black}
	const (
		whiteIndex = 0
		blackIndex = 1
	)
	const (
		cycles  = 5
		res     = 0.001
		size    = 100
		nframes = 64
		delay   = 8
	)
	freq := rand.Float64() * 3.0
	anim := gif.GIF{LoopCount: nframes}
	phase := 0.0

	for range nframes {
		rect := image.Rect(0, 0, 2*size+1, 2*size+1)
		img := image.NewPaletted(rect, palette)
		for t := 0.0; t < cycles*2*math.Pi; t += res {
			x := math.Sin(t)
			y := math.Sin(t*freq + phase)
			img.SetColorIndex(size+int(x*size+0.5), size+int(y*size+0.5), blackIndex)
		}
		phase += 0.1
		anim.Delay = append(anim.Delay, delay)
		anim.Image = append(anim.Image, img)
	}
	gif.EncodeAll(out, &anim)
}

func curl(urls []string) {
	for _, url := range urls {
		resp, err := http.Get(url)
		fmt.Println(resp, err)
	}
}

func fetch(url string, ch chan<- string) {
	start := time.Now()
	resp, err := http.Get(url)
	if err != nil {
		ch <- fmt.Sprint(err) // send to channel ch
		return
	}
	nbytes, err := io.Copy(io.Discard, resp.Body)
	resp.Body.Close() // don't leak resources
	if err != nil {
		ch <- fmt.Sprintf("while reading %s: %v", url, err)
		return
	}
	secs := time.Since(start).Seconds()
	ch <- fmt.Sprintf("%.2fs  %7d  %s", secs, nbytes, url)
}

func fetchAll() {
	start := time.Now()
	ch := make(chan string)
	for _, url := range os.Args[1:] {
		go fetch(url, ch) // start a goroutine
	}
	// channel is just a queue
	for range os.Args[1:] {
		fmt.Println(<-ch) // receive from channel ch
	}
	fmt.Printf("%.2fs elapsed\n", time.Since(start).Seconds())
}

// pc[i] is the population count of i.
var pc [256]byte = func() (pc [256]byte) {
	for i := range pc {
		pc[i] = pc[i/2] + byte(i&1)
	}
	return
}()

// PopCount returns the population count (number of set bits) of x.
func PopCount(x uint64) int {
	fmt.Println(pc)
	return int(pc[byte(x>>(0*8))] +
		pc[byte(x>>(1*8))] +
		pc[byte(x>>(2*8))] +
		pc[byte(x>>(3*8))] +
		pc[byte(x>>(4*8))] +
		pc[byte(x>>(5*8))] +
		pc[byte(x>>(6*8))] +
		pc[byte(x>>(7*8))])
}

func unicodeTest() {
	s := "Hello, 世界"
	fmt.Println(len(s))                    // "13"
	fmt.Println(utf8.RuneCountInString(s)) // "9"
	for i := 0; i < len(s); {
		r, size := utf8.DecodeRuneInString(s[i:])
		fmt.Printf("%d\t%c\n", i, r)
		i += size
	}

	for i, r := range s {
		fmt.Printf("%d\t%c\n", i, r)
	}

	n := 0
	for range s {
		n++
	}

	fmt.Println(utf8.RuneCountInString(s))

	s = "世界"
	fmt.Printf("% x\n", s)
	r := []rune(s) // utf-8 to unicode(unicode code point is 32 bit)
	fmt.Printf("%x\n", r)
	fmt.Println(string(65))     // "A", not "65"
	fmt.Printf(string(r)) // unicode to utf-8
}

func arr() {
	var a [3]int = [...]int{0, 1, 2} // array
	as := a[0:1]
	as = append(as, 10) // overwrite a[1]
	as = append(as, 11) // overwrite a[2]
	as = append(as, 12) // allocate new buffer to save slice value
	s := []int{0, 1, 2}
	s = append(s, 10)
	fmt.Println(a, as, s)

	var runes []rune
	for _, r := range "Hello, 世界" {
		runes = append(runes, r)
	}
	fmt.Printf("%q\n", runes) // "['H' 'e' 'l' 'l' 'o' ',' ' ' '世' '界']"
}

func mapTest() {
	// ages := make(map[string]int)
	ages := map[string]int{
		"a": 1,
		"b": 2,
	}
	fmt.Println(ages)
	fmt.Println(ages["a"])

	for name, age := range ages {
		fmt.Printf("%s\t%d\n", name, age)
	}

	if age, ok := ages["bob"]; !ok {
		fmt.Println("not ok, bob: " + strconv.Itoa(age))
	}

}
