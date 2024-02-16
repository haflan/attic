public class DCT {
    private int M;
    Matrix A;   // transformation matrix
    Matrix AT;   // transported

    public DCT(int blockSize){
        this.M = blockSize;
        this.A = new Matrix(blockSize);
        this.generateTransformMatrix();
        this.AT = this.A.getTransposed();
    }

    public void generateTransformMatrix(){
        for (int c = 0; c < M; c++){
            A.set(0, c, Math.sqrt(1.0/M) );
        }
        double a = Math.sqrt(2.0/M);
        for (int r = 1; r < M; r++){
            for (int c = 0; c < M; c++) {
                A.set(r, c, a*Math.cos( (2*c + 1) * r*Math.PI / (2 * M)) );
            }
        }
    }

    public Matrix transform(Matrix x){
        return this.A.multiply(x).multiply(this.AT);
    }

    @Override
    public String toString(){
        return this.A.toString();
    }
}
