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
            // ȣ��Ʈ �̸����� �˻��Ǵ� ù ��° IP4 �ּ� Ȯ��
            string hostName = Dns.GetHostName();
            ipHostEntry = Dns.GetHostEntry(hostName);
            foreach (IPAddress address in ipHostEntry.AddressList)
            {   //ȣ��Ʈ �̸��� ���� IP �ּ� ����� �ݺ��ϸ鼭, IPv4 �ּ����� Ȯ���մϴ�.
                //AddressFamily.InterNetwork�� IPv4 �ּ� �йи��� ��Ÿ���ϴ�.
                if (address.AddressFamily == AddressFamily.InterNetwork)
                {
                    serverIP = address.ToString();
                    break;
                    //ù ��°�� �߰ߵ� IPv4 �ּҸ� serverIP ������ �����ϰ� ������ �����մϴ�.
                }
            }
            listBox1.Items.Add("Server IP: " + serverIP);

            data = new byte[1048576 * 10]; // 1MB * 10
                                           // Ŭ���̾�Ʈ�� ���� ���۵Ǵ� PNG ������ ��Ȳ�� ���� 4MB�� �ѱ⵵ �Ѵ�.
            dataSizeFromClient = new byte[sizeof(int)];
        }

        private void ThreadProc()
        {
            //IPAddress.Any�� ��� ��Ʈ��ũ �������̽����� Ŭ���̾�Ʈ�� ������ �����ϰڴٴ� ���� �ǹ�
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
                        // Ŭ���̾�Ʈ�� ���� �޾ƾ� �� ������ ������ ���� Ȯ��.
                        networkStream.Read(dataSizeFromClient, 0, dataSizeFromClient.Length);
                        expectedDataSize = BitConverter.ToInt32(dataSizeFromClient, 0);
                        //listBox1.Items.Add("Expected data size: " + (expectedDataSize / 1024).ToString() + "KB");

                        // ������ ����.                        
                        do
                        {
                            // Ŭ���̾�Ʈ�� ���� �޾ƾ� �� ������ ������ ��ŭ�� ��Ȯ�� �޴´�.
                            receivedDataSize += networkStream.Read(data, receivedDataSize, expectedDataSize - receivedDataSize);
                            // Reads data from the NetworkStream and stores it to a byte array.
                        } while (expectedDataSize != receivedDataSize);
                        //while (networkStream.DataAvailable);
                        // NetworkStream.DataAvailable �� ��Ʈ��ũ ��Ȳ�� ���� ��Ȯ���� ���� ���ɼ��� ũ��.

                        //listBox1.Items.Add("Data size: " + (receivedDataSize / 1024).ToString() + "KB");
                        // Ŭ���̾�Ʈ�� ���� ���� ������ ����� ���� ������(10MB) ���� ũ�ٸ� ���� ����� �÷��� �Ѵ�.
                        // �׷��� ������ NetworkStream.Read()���� ArgumentOutOfRangeException ���� �߻�.
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
            // Ŭ���̾�Ʈ ���� ��⸦ ���� ������ ����(������� 1���� �����Ѵ�)
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
                // TcpListener�� ���� ��Ű�� �ʰ� Ŭ���̾�Ʈ ���� ���� ���α׷���
                // �����Ϸ� �ϸ� �����尡 ������� �ʾ� ���α׷��� ������� �ʴ´�.
                listener.Stop();

                // Ŭ���̾�Ʈ�� ����� �����带 �������� ������ ���α׷��� �����ص�
                // ��׶��忡�� ��� ����ȴ�.                
                thread.Interrupt();
            }
        }

    }
}