import java.io.*;
import java.net.*;
import java.util.Date;

public class Client {

  public static void main(String[] args) {
    int channel2ServerPort = 6789;
    String serverName=null;
    int channel1ServerPort=0;
    int UDPPort=0;
    if (args.length > 0) {
      serverName = args[0];
      channel1ServerPort = Integer.parseInt(args[1]);
      UDPPort = Integer.parseInt(args[2]);
    }
   /* else {
      serverName = "localhost";
      channel1ServerPort = 6790;
      UDPPort = 9876;
    }*/
    String Status;
    Socket channel1ClientSocket;
    try {
      InetAddress address=null;
      channel1ClientSocket = new Socket(InetAddress.getByName(serverName),channel1ServerPort);

      DataOutputStream outToServer = new DataOutputStream
          (channel1ClientSocket.
           getOutputStream());

      BufferedReader inFromServer = new BufferedReader(new
          InputStreamReader(
          channel1ClientSocket.getInputStream()));

      // Send [Ready|tcp-port2],where tcp-port2 is Channel_2 server port.
          String port;
      port = "READY|" + channel2ServerPort;

      outToServer.writeBytes(port + "\n");

      System.out.println("1) " + port);
      // Receive [Server-TIMESTAMP|ts1]

      String ts1, temp, header2;
      temp = inFromServer.readLine();
      header2 = temp.substring(0, 16);
      if (header2.equals("SERVER-TIMPSTAMP")) {
        ts1 = temp.substring(17);
        System.out.println("2) " + temp);
        Date ts2 = new Date();
        String t;
        t = "CLIENT-TIMESTAMP|" + ts1 + "|" + ts2.getTime();
        outToServer.writeBytes(t + "\n");
        System.out.println("3) " + t);

        /******************************************************************
         ***********/
        // Channel 2 Communincation
        // here I'm the Server
        ServerSocket cahnnel2ServerSocket = null;
        try {
          cahnnel2ServerSocket = new ServerSocket
              (channel2ServerPort);
          cahnnel2ServerSocket.setSoTimeout(100);
          while (true) {
            String ECHO;
            String ECHOReply;

            Socket channel2ConnectionSocket =
                cahnnel2ServerSocket.accept();

            BufferedReader inFromChannel2Client = new
                BufferedReader(new
                               InputStreamReader
                               (channel2ConnectionSocket.getInputStream()));

            DataOutputStream outToChannel2Client = new
                DataOutputStream(
                channel2ConnectionSocket.getOutputStream());

            String header4, message;
            message = inFromChannel2Client.readLine();
            header4 = message.substring(0, 19);
            if (header4.equals("CLIENT-ECHO-REQUEST")) {
              //Receive ECHO.
              ECHO = message.substring(20);
              System.out.println("4.1) CLIENT-ECHO-REQUEST|" +
                                 ECHO);

              //Send ECHO Reply.
              ECHOReply = "SERVER-ECHO-REPLY|" + ECHO;
              outToChannel2Client.writeBytes(ECHOReply + "\n");
              outToChannel2Client.flush();
              System.out.println("4.2) " + ECHOReply);

              //Receive OK
              System.out.println("4.3) " +
                                 inFromChannel2Client.readLine());

              channel2ConnectionSocket.close();
              cahnnel2ServerSocket.close();

            }
          }
        }
        catch (Exception ex1) {
        }
        //Receive [you can be a server] or[you can not be a server]
        String canbeServer;
        canbeServer = inFromServer.readLine();
        System.out.println("5) " + canbeServer);
        if (canbeServer.equals("You can be a server")) {
          outToServer.writeBytes("I can be a server\n");
          System.out.println("6) I can be a server");
        }
        else {
          outToServer.writeBytes("I can't be a server\n");
          System.out.println("6) I can't be a server");
        }

        /******************************************************************
         **/
        //channel3 start
        int count = 0;
        try {
          DatagramSocket channel3Client = new DatagramSocket();
          System.out.println("7) Channel 3 communications");
          InetAddress IPAddress = InetAddress.getByName
              ("localhost");

          byte[] sendData = new byte[1024];
          byte[] receiveData = new byte[1024];

          for (int i = 0; i < 10; i++) {
            String udpECHO = "ECHO-REQUEST|message\n";

            sendData = udpECHO.getBytes();

            DatagramPacket sendPacket = new DatagramPacket
                (sendData,
                 sendData.length, IPAddress, UDPPort);

            channel3Client.send(sendPacket);

            DatagramPacket receivePacket = new DatagramPacket
                (receiveData,
                 receiveData.length);

            channel3Client.setSoTimeout(2);
            int j = 0;
            try {
              channel3Client.receive(receivePacket);
              String msg = new String(receivePacket.getData());
              String header5;
              header5 = msg.substring(0, 10);
              if (header5.equals("ECHO-REPLY"))
                count++;
            }
            catch (Exception ex) {
              for (j = 0; j < 3; j++) {
                channel3Client.send(sendPacket);
                try {
                  channel3Client.receive(receivePacket);
                  String msg = new String(receivePacket.getData());
                  String header5;
                  header5 = msg.substring(0, 10);
                  if (header5.equals("ECHO-REPLY")) {
                    count++;
                    break;
                  }
                }
                catch (Exception ex2) {
                }
              }
            }

            if (j >= 3) {
              break;
            }
          }
          channel3Client.close();
          //close channel3
        }
        catch (Exception ex) {
        }

        if (count == 0) {
          Status = "FAILED";
        }
        else if (count == 10) {
          Status = "DONE";
        }
        else
          Status = "TIMEOUT";
        boolean flag = false;
        outToServer.writeBytes("UDP-ECHO|" + Status + "\n");
        System.out.println("8) " + "UDP-ECHO|" + Status);
        if (count > 0) {
          String m, header6, calc;
          calc = "UDP-ECHO|" + Status;
          m = inFromServer.readLine();
          header6 = m.substring(0, calc.length());

          if (header6.equals(calc)) {
            System.out.println("9) " + m);
            System.out.println("10) " + inFromServer.readLine());
            flag = true;
          }
        }
        if (flag == false) {
          System.out.println("10) " + inFromServer.readLine());
        }
        channel1ClientSocket.close();
      }
    }
    catch (IOException ex) {
      System.out.println("Cann't find TCP Server.");
    }

  }
}