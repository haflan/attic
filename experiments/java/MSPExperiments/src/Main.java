public class Main {

    public static void main(String[] args){
        DCT transform  = new DCT(8);
        MatrixFile matrixFile = new MatrixFile("matrix.mat");
        Matrix matrix = matrixFile.getMatrix();
        System.out.println(transform);
        System.out.println(matrix);
        System.out.println(transform.transform(matrix));
    }
}
