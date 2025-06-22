def case1(l:list, result=[]):
    print("l:", hex(id(l)), " result:", hex(id(result)))
    for i in l:
        result.append(i)
    return result

print(case1([1,2,3])) # [1,2,3]
print(case1([1,2,3,4])) # [1,2,3,1,2,3]

