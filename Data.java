import java.util.Arrays;

public class Data {

	@Override
	public String toString() {
		return "Data [className=" + className + ", feature=" + Arrays.toString(feature) + "]";
	}

	private String className;
	private double[] feature;

	public Data(double[] feature, String className) {
		// TODO 自動生成されたコンストラクター・スタブ
		this.feature = feature;
		this.className = className;
	}

	public String getClassName() {
		return className;
	}
	
	public double[] getFeature() {
		return feature;
	}
	
	


}
