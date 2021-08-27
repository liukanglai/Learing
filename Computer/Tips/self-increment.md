# talk about the a[++i]

such as:

        while(i < strlen(a)){
            if(a[++i]) a[i] = 0;
        }
        
        printf("%d %d %d",i++,++i,i);
        
- if i = strlen(a) - 1, then a[++j] is wrong.
- and if use a[i++], the a[i] will be wrong.
- the most important thing is that the ++ is transported in function.

so the best way is to use i++ out of the a[]...
