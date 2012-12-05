import java.awt.BorderLayout;
import java.awt.GridLayout;
import java.awt.TextArea;
import java.io.*;
import java.nio.*;
import java.nio.channels.*;
import java.nio.charset.*;
import java.net.*;
import java.util.*;
import javax.swing.*;



import javax.swing.JFrame;

public class EchoServer extends JFrame{
  private JTextField info = new JTextField();
  private TextArea text = new TextArea(20,15);
  private Selector selector = null;
  private ServerSocketChannel serverSocketChannel = null;
  private int port = 12345;
  private Charset charset=Charset.forName("GBK");
  private Thread server = new Thread(){
	  public void run(){
		  try {
			service();
		} catch (IOException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
	  }
  };

  public EchoServer()throws IOException{
    selector = Selector.open();
    serverSocketChannel= ServerSocketChannel.open();
    serverSocketChannel.socket().setReuseAddress(true);
    serverSocketChannel.configureBlocking(false);
    serverSocketChannel.socket().bind(new InetSocketAddress(port));
    JPanel p = new JPanel();
    p.setLayout(new GridLayout(1,1));
    p.add(info);
    add(p, BorderLayout.CENTER);
    add(text,BorderLayout.SOUTH);
    server.start();
    info.setText("服务器已启动,记录文件为ReceivedInfo.txt");
  }

  public void service() throws IOException{
    serverSocketChannel.register(selector, SelectionKey.OP_ACCEPT );
    while (selector.select() > 0 ){
      Set<SelectionKey> readyKeys = selector.selectedKeys();
      Iterator<SelectionKey> it = readyKeys.iterator();
      while (it.hasNext()){
         SelectionKey key=null;
         try{
            key = (SelectionKey) it.next();
            it.remove();

            if (key.isAcceptable()) {
              ServerSocketChannel ssc = (ServerSocketChannel) key.channel();
              SocketChannel socketChannel = (SocketChannel) ssc.accept();
              text.append("接收到客户连接，来自:" +
                                 socketChannel.socket().getInetAddress() +
                                 ":" + socketChannel.socket().getPort()+"\r\n");
              socketChannel.configureBlocking(false);
              ByteBuffer buffer = ByteBuffer.allocate(1024);
              socketChannel.register(selector,
                                     SelectionKey.OP_READ,buffer);
            }
            if (key.isReadable()) {
                receive(key);
            }
        }catch(IOException e){
           e.printStackTrace();
           try{
               if(key!=null){
                   key.cancel();
                   key.channel().close();
               }
           }catch(Exception ex){e.printStackTrace();}
        }
      }//#while
    }//#while
  }
  public void receive(SelectionKey key)throws IOException{
    ByteBuffer buffer=(ByteBuffer)key.attachment();

    SocketChannel socketChannel=(SocketChannel)key.channel();
    ByteBuffer readBuff= ByteBuffer.allocate(32);
    socketChannel.read(readBuff);
    readBuff.flip();

    buffer.limit(buffer.capacity());
    buffer.put(readBuff);
    writeInfo(buffer,socketChannel);
  }
  
  public boolean writeInfo(ByteBuffer buffer,SocketChannel socketChannel){
	  buffer.flip();
	  String data=decode(buffer);
	  if(data.indexOf("\r\n")==-1)
		  return false;
	  String outputData=data.substring(0,data.indexOf("\n")+1);
	  String outputInfo = socketChannel.socket().getInetAddress().toString()+":"+outputData;
	  try {
		  InfoWriter.writeInfo(outputInfo);
	  } catch (IOException e) {
		 
		  e.printStackTrace();
		  return false;
	  }
	  ByteBuffer temp=encode(outputData);
	  buffer.position(temp.limit());
      buffer.compact();
	  return true;
  }

  public String decode(ByteBuffer buffer){  //解码
    CharBuffer charBuffer= charset.decode(buffer);
    return charBuffer.toString();
  }
  public ByteBuffer encode(String str){  //编码
    return charset.encode(str);
  }

  public static void main(String args[])throws Exception{
	JFrame f = new EchoServer();
	f.setTitle("LANShieldServer1.0");
	f.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
	f.setSize(350,400);
	f.setVisible(true);
  }
}
