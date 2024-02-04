import java.io.*;
import java.net.*;
import java.util.Date;

public class Server {
  public static void main(String[] args) {

    ServerSocket cahnnel1ServerSocket = null;
    int serverPort=0;
    int UDPPort=0;

    if (args.length>0){
      serverPort = Integer.parseInt(args[0]);
      UDPPort = Integer.parseInt(args[1]);
    }
    else
    {
      //serverPort = 6790;
      //UDPPort = 9876;
    }
    try {
      cahnnel1ServerSocket = new ServerSocket(serverPort);
      cahnnel1ServerSocket.setSoTimeout(10000);

      while (true) {

        int channel2ServerPort;

        Socket connectionSocket = cahnnel1ServerSocket.accept();

        BufferedReader inFromClient = new BufferedReader(new InputStreamReader(
            connectionSocket.getInputStream()));

        DataOutputStream outToClient = new DataOutputStream(connectionSocket.
            getOutputStream());

        // receive Channel2 server port from client
        String receivedPort, port, header1;
        receivedPort = inFromClient.readLine();
        header1 = receivedPort.substring(0, 5);
        if (header1.equals("READY")) {
          port = receivedPort.substring(6);
          channel2ServerPort = Integer.parseInt(port);

          System.out.println("1) " + receivedPort);

          // Send Server-TIMESTAMP
          Date ts1 = new Date();
          String serverTimeStamp, time;
          time = "" + ts1.getTime();
          serverTimeStamp = "SERVER-TIMPSTAMP|" + time;
          outToClient.writeBytes(serverTimeStamp + "\n");
          System.out.println("2) " + serverTimeStamp);
          String ts2, ts2header = "";
          String[] header3;
          ts2 = inFromClient.readLine();
          ts2header = ts2.substring(0, 30);
          if (ts2header.equals("CLIENT-TIMESTAMP|" + time)) {
            System.out.println("3) " + ts2);

                /************************************************************************/

// channel 2 communication
// here I'm the client
            Socket channel2Client;
            boolean canBeServer = false;
            try {
              channel2Client = new Socket("localhost", channel2ServerPort);
              canBeServer = true;

              DataOutputStream outToChannel2Server = new DataOutputStream(
                  channel2Client.getOutputStream());

              BufferedReader inFromChannel2Server = new BufferedReader(new
                  InputStreamReader(channel2Client.getInputStream()));

              // send [Client-ECHO-Request|msg]
              String ECHO;
              ECHO = "Hello Server";
              outToChannel2Server.writeBytes("CLIENT-ECHO-REQUEST|" + ECHO +
                                             "\n");
              outToChannel2Server.flush();
              System.out.println("4.1) CLIENT-ECHO-REQUEST|" + ECHO);

              //Receive Reply
              String Reply, message, header4;
              message = inFromChannel2Server.readLine();
              header4 = message.substring(0, 17);
              if (header4.equals("SERVER-ECHO-REPLY")) {
                Reply = message.substring(18);
                System.out.println("4.2) SERVER-ECHO-REPLY|" + Reply);

                // Send OK
                if (ECHO.equals(Reply)) {
                  outToChannel2Server.flush();
                  outToChannel2Server.writeBytes("OK\n");
                  System.out.println("4.3) OK");
                  channel2Client.close();
                }
                else {
                  outToChannel2Server.flush();
                  outToChannel2Server.writeBytes("NOT-OK\n");
                  System.out.println("4.3) NOT-OK");
                  channel2Client.close();
                }
              }
            }
            catch (IOException ex1) {
              canBeServer = false;
            }

            //Send [you can be a server] or [you can't be a server]
            if (canBeServer == true) {
              outToClient.writeBytes("You can be a server\n");
              System.out.println("5) You can be a server");
            }
            else {
              outToClient.writeBytes("You can't be a server\n");
              outToClient.flush();
              System.out.println("5) You can't be a server");
            }

            //Receive his reply
            System.out.println("6) " + inFromClient.readLine());

                /*********************************************************************/
            //channle 3 start
            DatagramSocket channle3Server = null;
            int count = 0;
            try {
              channle3Server = new DatagramSocket(UDPPort);
              channle3Server.setSoTimeout(100);
              System.out.println("7) channle 3 communication");
              byte[] receiveData = new byte[1024];
              byte[] sendData = new byte[1024];
              while (true) {
                DatagramPacket receivePacket = new DatagramPacket(receiveData,
                    receiveData.length);
                channle3Server.receive(receivePacket);
                String msg = new String(receivePacket.getData());
                String header5;
                header5 = msg.substring(0, 12);
                if (header5.equals("ECHO-REQUEST")) {
                  count++;
                  InetAddress IPAddress = receivePacket.getAddress();
                  int port2 = receivePacket.getPort();
                  String replyMsg = "ECHO-REPLY|message\n";
                  sendData = replyMsg.getBytes();
                  DatagramPacket sendPacket = new DatagramPacket(sendData,
                      sendData.length, IPAddress, port2);
                  channle3Server.send(sendPacket);
                }
              }
            }
            catch (Exception ex) {
              try {
                channle3Server.close();
              }
              catch (Exception ex1) {
              }
            }
            String status, m, header6;
            m = inFromClient.readLine();
            header6 = m.substring(0, 8);
            if (header6.equals("UDP-ECHO")) {
              status = m.substring(9);
              System.out.println("8) " + "UDP-ECHO|" + status);
              if (!status.equals("FAILED")) {
                outToClient.writeBytes("UDP-ECHO|" + status + "|" + count +
                                       "\n");
                outToClient.flush();
                System.out.println("9) " + "UDP-ECHO|" + status + "|" + count);
              }

              outToClient.writeBytes("TEST_WELL_DONE\n");
              outToClient.flush();
              System.out.println("10) TEST_WELL_DONE");
            }
          }
        }
      }
    }
    catch (Exception ex) {
      try {
        cahnnel1ServerSocket.close();
      }
      catch (Exception e) {
        System.out.println("Can't be a server.");
      }
    }

  }

}
