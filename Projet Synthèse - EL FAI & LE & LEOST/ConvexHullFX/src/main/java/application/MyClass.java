package application;

public class MyClass {
    public native void findConvexHull(String infilename, String outfilename, int algo, int sort);

    static {
        System.loadLibrary("mylibrary");
    }
}