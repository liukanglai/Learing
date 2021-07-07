# file

    File file0 = new file("1.txt"); // In idea, in the module
    File file1 = new file("/home/liukanglai/");
    File file2 = new file("/home", "liukanglai");
    File file3 = new file(file2, "liukanglai");


## Methods

    public String getAbsolutePath()
    public String getPath()
    public String getName()
    public String getParent() // if not, return null
    public long length() // file length(byte num), can't get the length of the directory's name.
    public long lastModified() // last time(ms), (new Date(file.lastModified()))

    // For directory
    public String[] list() // get all the files in the directory or directory's name array
    public File[] listFiles()  // get all the files in the directory or directory's file array

    String[] list file.list();
    for(String s : list) {
        System.out.println(s); // just name
    }
    File[] files file.listFiles();
    for(File f : files) {
        System.out.println(f); // AbsolutePath
    }

    public boolean renameTo(File dist)
    // file exists, but file1 can't exist.
    file.renameTo(file1);

    public boolean isDirectory()
    public boolean isFile()
    public boolean exists() // !!! first do
    public boolean canRead()
    public boolean canWrite()
    public boolean isHidden()

    public boolean createNewFile()
    public boolean mkdir()
    public boolean mkdirs() // for parent
    public boolean delete()

##
