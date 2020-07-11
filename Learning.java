import java.io.File;
import java.io.FileNotFoundException;
import java.util.Arrays;
import java.util.LinkedList;
import java.util.Scanner;

public class Learning {
	
	private final static int FEATURE_NUM = 4; //特徴ベクトルの数
	private final static double RHO = 0.5; //ρの値
	public static double linearDiscriminantFunction(double[] f,double[] w) {
		double g = 0;
		for(int i = 0;i < f.length; i++){
			g += f[i]*w[i];
		}
		return g;
	}
	
	
	public static void readData(LinkedList<Data> d) {
		try{	
			File file = new File("bezdekIris.data");
			Scanner sc = new Scanner(file);
			sc.useDelimiter(",|\n");
			while(sc.hasNextLine()){
				double[] feature = new double[FEATURE_NUM+1];		
				String className = new String();
				feature[0] = 1;
				String buf = sc.next();
				feature[1] = Double.parseDouble(buf);
				buf = sc.next();
				feature[2] = Double.parseDouble(buf);
				buf = sc.next();
				feature[3] = Double.parseDouble(buf);
				buf = sc.next();
				feature[4] = Double.parseDouble(buf);
				className = sc.next();
				d.add(new Data(feature,className));
				sc.nextLine();
//				System.out.println("fV = "+Arrays.toString(feature)+System.lineSeparator()+"class : "+className)
			}
			sc.close();
		}catch(FileNotFoundException e){
			e.printStackTrace();
		}	
	}
	
	public static void main(String[] args) {
		// TODO 自動生成されたメソッド・スタブ
	 LinkedList<Data> dataList = new LinkedList<>();
	 readData(dataList);
	/* for(Data d: dataList){
		 System.out.println(d.toString());
	 }*/
	 String[] className ={"Iris-setosa","Iris-versicolor","Iris-virginica"};
	 int i = 0;
	 boolean incomplete = true;
	 double[] weight = new double[FEATURE_NUM+1];
	 for(int j = 0; j < FEATURE_NUM+1; j++){
		 weight[j] = 0.1;
	 }
	 do{
		 incomplete = false;
		 double g = 0;
		 for(Data d:dataList){
			 g = linearDiscriminantFunction(d.getFeature(),weight);
			 if(g < 0 && d.getClassName().equals(className[0])){
				 incomplete = true;
				 for(int j = 0; j< weight.length; j++){
					 weight[j] += RHO*d.getFeature()[j];
				 }
			 }else if(g > 0 && d.getClassName().equals(className[1])){
				 incomplete = true;
				 for(int j = 0; j< weight.length; j++){
					 weight[j] -= RHO*d.getFeature()[j];
				 }
			 }	
		 }
		 i++;
	 }while(i < 1000 && incomplete);

	 if(i == 1000){
		 System.out.println("線形分離不可能と判断");
		 System.out.println(Arrays.toString(weight));
	 }else{

		 System.out.println("線形分離可能");
		 System.out.println(Arrays.toString(weight));
	 }

	}

}
