import java.io.*;


public class InfoWriter {
    private static String filePath = "ReceivedInfo.txt";
    public static void writeInfo(String info)throws IOException{
    	PrintWriter out = new PrintWriter(new BufferedWriter(new FileWriter(filePath,true)));
    	out.print(info);
    	out.close();
    }
}
