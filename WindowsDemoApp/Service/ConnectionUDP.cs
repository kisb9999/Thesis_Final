using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Net.Sockets;
using System.Net;

namespace WindowsDemoApp.Service
{
    public class ConnectionUDP
    {
        private const int UDP_PORT = 41234;
        private const string IP = "79.120.218.116";
        private const string name = "DashBoard Demo";
        private Socket socket;
        private IPEndPoint endPoint;
        private string imei;
        private string ccid;


        public ConnectionUDP(string imei, string ccid)
        {
            endPoint = new IPEndPoint(IPAddress.Parse(IP), UDP_PORT);
            socket = new Socket(AddressFamily.InterNetwork, SocketType.Dgram, ProtocolType.Udp);
            this.imei = imei;
            this.ccid = ccid;
        }

        public void SendData(string temperature, string humidity, string als)
        {
            string str = $"{{\"IMEI\": \"{imei}\", \"msgref\" : \"{name} ICCID:{ccid} IMEI:{imei} \", \"payload\" : \"|T{temperature}|H{humidity}|A{als}|\", \"gpsdata\" : \"10,20\"}}";
            byte[] data = Encoding.UTF8.GetBytes(str);

            try
            {
                socket.SendTo(data, endPoint);
            }
            catch (Exception ex)
            {
                Console.WriteLine(ex.ToString());
            }
        }
    }
}
