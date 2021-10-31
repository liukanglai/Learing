# fork


- 1, 2, 4, 8, 16, 32... seems 2^(n-1)?
- n's fork(), n>2, have process: `1+n+(n-1)+2(n-2)+4(n-3)+8(n-4)+...+2^(n-2)`, it equals to 2^(n-1)?
- for 2^(n-1), think: father + (n-1) sons look as (n-1) fork(), and the biggest son will have sons just like (n-1) fork(). So the n fork() equal to 2 * (n-1) fork().

# about the memory

- the sons just copy the values in father.
- I discover the values in different processes have the same address, ?
