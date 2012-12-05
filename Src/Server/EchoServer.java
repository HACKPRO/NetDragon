import java.awt.BorderLayout;
import java.awt.GridLayout;
import java.awt.TextArea;
import java.io.*;
import java.nio.*;
import java.nio.channels.*;
import java.nio.charset.*;
import java.net.*;
import java.util.*;
import java.util.logging.Level;
import java.util.logging.Logger;
import javax.swing.*;
import java.sql.*;


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
    info.setText("服务器已启动");
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
      String attacker = outputData.substring(0,outputData.indexOf("#"));
      String types = outputData.substring(outputData.indexOf("#")+1,outputData.indexOf("#")+2);
      int type = Integer.parseInt(types);
      
	  String sender1 = socketChannel.socket().getInetAddress().toString();
      String sender = sender1.substring(sender1.indexOf("/")+1);
      Connection con = null;
	  try {
		  ConnectionProvider cp = new ConnectionProvider();

          PreparedStatement stmt = null;
          con = cp.getConnection();
         
          String sql = "insert into info(ID,SENDER,ATTACKER,TYPE,TIME)values(?,?,?,?,?)";
          stmt = con.prepareStatement(sql);
          stmt.setInt(1,0);
          stmt.setString(2, sender);
          stmt.setString(3, attacker);
          stmt.setInt(4,type);
          stmt.setDate(5,new java.sql.Date(new java.util.Date().getTime()));
          stmt.execute();
	  } catch (Exception e) {
		 
		  e.printStackTrace();
		  return false;
	  }finally{
          try{
              con.close();
          }catch(SQLException e){e.printStackTrace();}
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
      SwingUtilities.invokeLater(new Runnable(){
          public void run(){
              JFrame f=null;
                try {
                    f = new EchoServer();
                } catch (IOException ex) {
                    Logger.getLogger(EchoServer.class.getName()).log(Level.SEVERE, null, ex);
                }
              f.setTitle("NetShieldServer");
              f.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
              f.setSize(350,400);
              f.setVisible(true);
          }
      });
  }
}
