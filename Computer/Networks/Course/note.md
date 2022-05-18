# CRC

- 模 2 除法 (异或)
- G 首必一(n + 1 bit) （生成 G 的方法）
- R 前补 0(n bit) (CRC)

# when bits wrongs.

- G = 1001(能检测出奇数比特差错?)

- D = A + B
- if(D mod G) == if(A mod G + B mod G)

- 1 bit: 加减 2^i -> 1 个 1
- 2 bits: 加减 2^i and 2^j (i != j) 2 个 1
- ...

- 1,2,4,8... 对 9 mod

- is XOR, not mod !!!

- so ??? ???
