> close!!!


# InputStream  OutputStream (字节)

# Writer Reader (字符, 不能处理非文本文件)

# 节点流

1. File + InputStream/OutputStream

- read(byte[] c)
- write(byte[] c, 0, len)
- .newline(): 换行


2. File + Reader/Writer

- read(char[] c)
- write(char[] c, 0, len)

# 处理流 *+.


1. Buffered

> have a buffer area to store the data, to accelerate
- .flush(): refresh buffer, when full, do it automatically
    
        br.flush();
- .readLine() (BufferedReader): return the length or null in the end of the file, not include the line terminal(    enter),

    String read = br.readLine();

2. InputStreamReader isr = new InputStreamReader(file, "UTF-8");

> ASCII(American 7 bits), ISO8859-1(E, 8 bits), GB2312(chinese, 2 bytes), GBK(Chinese+), Unicode(2 bytes), UTF-8(1-4 bytes, chinese have 3 bytes)

3. System.in(InputStream)/out(PrintStream)
        
        InputStreamReader isr = new InputStreamReader(System.in)
        BufferedReader br = new BufferedReader(isr)
        
        System.setOut();
        System.setIn();

4. PrintStream/PrintWriter

        System.setOut(new PrintStream);

5. DataInputStream/DataOutputStream // 读取基本数据类型or字符串

- writeUTF("");
- writeInt(int a);
- flush(); // 内存写入磁盘

> use DataInputStream to read, other way to read will wrong, attention the data order.


# 对象流
