using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

using System.Threading;
using System.Net;
using System.Net.Sockets;
using System.IO;
//using static System.Runtime.InteropServices.JavaScript.JSType;

namespace Server
{
    public partial class Form1 : Form
    {
        TcpListener listener;
        TcpClient client;
        NetworkStream networkStream;
        MemoryStream memoryStream;
        Bitmap bitmap;
        IPHostEntry ipHostEntry;
        Thread thread;

        string serverIP;
        int serverPort;
        byte[] data;
        byte[] dataSizeFromClient;
        int receivedDataSize;
        int expectedDataSize;
        public Form1()
        {
            InitializeComponent();
            pictureBox1.SizeMode = PictureBoxSizeMode.StretchImage;

            serverPort = 7000;
            // 호스트 이름으로 검색되는 첫 번째 IP4 주소 확인
            string hostName = Dns.GetHostName();
            ipHostEntry = Dns.GetHostEntry(hostName);
            foreach (IPAddress address in ipHostEntry.AddressList)
            {   //호스트 이름에 대한 IP 주소 목록을 반복하면서, IPv4 주소인지 확인합니다.
                //AddressFamily.InterNetwork는 IPv4 주소 패밀리를 나타냅니다.
                if (address.AddressFamily == AddressFamily.InterNetwork)
                {
                    serverIP = address.ToString();
                    break;
                    //첫 번째로 발견된 IPv4 주소를 serverIP 변수에 저장하고 루프를 종료합니다.
                }
            }
            listBox1.Items.Add("Server IP: " + serverIP);

            data = new byte[1048576 * 10]; // 1MB * 10
                                           // 클라이언트로 부터 전송되는 PNG 파일은 상황에 따라 4MB를 넘기도 한다.
            dataSizeFromClient = new byte[sizeof(int)];
        }

        private void ThreadProc()
        {
            //IPAddress.Any는 모든 네트워크 인터페이스에서 클라이언트의 연결을 수신하겠다는 것을 의미
            listener = new TcpListener(IPAddress.Any, serverPort);
            //listener = new TcpListener(IPAddress.Parse("127.0.0.1"), serverPort);
            listener.Start();

            client = listener.AcceptTcpClient();
            listBox1.Items.Add("Client IP: " + client.Client.RemoteEndPoint.ToString().Split(':')[0]);
            networkStream = client.GetStream();

            while (true)
            {
                try
                {
                    receivedDataSize = 0;

                    if (networkStream.CanRead)
                    {
                        // 클라이언트로 부터 받아야 할 데이터 사이즈 정보 확인.
                        networkStream.Read(dataSizeFromClient, 0, dataSizeFromClient.Length);
                        expectedDataSize = BitConverter.ToInt32(dataSizeFromClient, 0);
                        //listBox1.Items.Add("Expected data size: " + (expectedDataSize / 1024).ToString() + "KB");

                        // 데이터 수신.                        
                        do
                        {
                            // 클라이언트로 부터 받아야 할 데이터 사이즈 만큼만 정확히 받는다.
                            receivedDataSize += networkStream.Read(data, receivedDataSize, expectedDataSize - receivedDataSize);
                            // Reads data from the NetworkStream and stores it to a byte array.
                        } while (expectedDataSize != receivedDataSize);
                        //while (networkStream.DataAvailable);
                        // NetworkStream.DataAvailable 은 네트워크 상황에 따라 정확하지 않을 가능성이 크다.

                        //listBox1.Items.Add("Data size: " + (receivedDataSize / 1024).ToString() + "KB");
                        // 클라이언트로 부터 받은 데이터 사이즈가 버퍼 사이즈(10MB) 보다 크다면 버퍼 사이즈를 늘려야 한다.
                        // 그렇지 않으면 NetworkStream.Read()에서 ArgumentOutOfRangeException 예외 발생.
                    }

                    //listBox1.Items.Add("Data received: " + (receivedDataSize / 1024).ToString() + "KB");
                    memoryStream = new MemoryStream(data, 0, receivedDataSize);
                    // Initializes a new non-resizable instance of the MemoryStream class
                    // based on the specified region (index) of a byte array.            
                    bitmap = new Bitmap(memoryStream);
                    pictureBox1.Image = bitmap;
                }
                catch (Exception e)
                {
                    listBox1.Items.Add(e.Message);
                    listBox1.Items.Add(e.StackTrace);
                    break;
                }
            }

            listener.Stop();
            client.Close();
            networkStream.Close();
            memoryStream.Close();
        }


        private void button1_Click(object sender, EventArgs e)
        {
            // 클라이언트 접속 대기를 위한 스레드 생성(스레드는 1개만 생성한다)
            if (thread == null || !thread.IsAlive)
            {
                thread = new Thread(new ThreadStart(ThreadProc));
                thread.Start();
                listBox1.Items.Add("Waiting for a client...");
            }
        }

        private void Form1_FormClosing(object sender, FormClosingEventArgs e)
        {
            if (thread != null && thread.IsAlive)
            {
                // TcpListener를 정지 시키지 않고 클라이언트 연결 대기시 프로그램을
                // 종료하려 하면 스레드가 종료되지 않아 프로그램이 종료되지 않는다.
                listener.Stop();

                // 클라이언트와 연결된 스레드를 종료하지 않으면 프로그램을 종료해도
                // 백그라운드에서 계속 실행된다.                
                thread.Interrupt();
            }
        }

    }
}