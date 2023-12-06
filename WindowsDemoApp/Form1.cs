using System.IO.Ports;
using System.Windows.Forms.DataVisualization.Charting;
using WindowsDemoApp.Service;

namespace WindowsDemoApp
{
    public partial class DemoApp : Form
    {
        private SerialPort myPort;
        private const string comPort = "COM5";
        private string data;
        private string[] temperature = new string[20];
        private string[] humidity = new string[20];
        private string[] als = new string[20];

        public DemoApp()
        {
            InitializeComponent();
            ch_test.ChartAreas[0].AxisX.Minimum = -1;
            ch_test.ChartAreas[0].AxisX.Maximum = 21;
            ch_test.ChartAreas[0].AxisX.Interval = 1;
            //initListView();
        }
        private void listView1_SelectedIndexChanged(object sender, EventArgs e)
        {

        }

        private void DemoApp_Load(object sender, EventArgs e)
        {
            //Set port attributes
            myPort = new SerialPort();
            myPort.BaudRate = 115200;
            myPort.PortName = comPort;
            myPort.Parity = Parity.None;
            myPort.DataBits = 8;
            myPort.StopBits = StopBits.One;
            myPort.DataReceived += DataReceived;

            //Start the timer
            update_timer.Start();
            trSendData.Start();

            //Try opening the COM port
            try
            {
                myPort.Open();
            }
            catch (Exception ex)
            {
                throw new Exception("Error during the COM3 port opening!");
            }
        }

        private void DataReceived(object sender, SerialDataReceivedEventArgs e)
        {
            data = myPort.ReadLine();
            if (data == null)
            {
                return;
            }
            Invoke(new EventHandler(DisplayData));
        }

        private void DisplayData(object sender, EventArgs e)
        {
            ListViewItem item = new ListViewItem(data, 0);
            //Gets the important infromation from the displayed data.
            if (data.Contains("I2C"))
            {
                int idx = data.IndexOf("Temperature");
                int startidx = idx + 13;
                string temp_data = data.Substring(startidx, 5);
                AddToArray(temperature, temp_data);

                idx = data.IndexOf("Humidity");
                startidx = idx + 10;
                string hum_data = data.Substring(startidx, 5);
                AddToArray(humidity, hum_data);
            }
            else if (data.Contains("GPIO"))
            {
                int idx = data.IndexOf("ADC2");
                int startidx = idx + 11;
                int i = startidx;
                while(data[i] != ' ')
                {
                    i++;
                }
                string als_data = data.Substring(startidx, i - startidx);
                AddToArray(als, als_data);
            }
            listView1.Items.Add(item);
        }

        private void DemoApp_FormClosing(object sender, FormClosingEventArgs e)
        {
            try
            {
                myPort?.Close();
            }
            catch 
            {
                throw new Exception("Something went wrong when trying to close the COM3 port!");
            }
        }

        private void UpdateCharts()
        {
            ch_test.Series["Temperature"].Points.Clear();
            ch_test.Series["Humidity"].Points.Clear();
            ch_test.Series["ALS"].Points.Clear();

            //Temperature and humidity data always comes together, so one loop is enough.
            for (int i = 0; i < temperature.Length; i++)
            {
                ch_test.Series["Temperature"].Points.AddXY(i, temperature[i]);
                ch_test.Series["Humidity"].Points.AddXY(i, humidity[i]);
            }
            for (int i = 0; i < als.Length; i++)
            {
                ch_test.Series["ALS"].Points.AddXY(i, als[i]);
            }
        }

        private void AddToArray(string[] array, string data)
        {
            //If the array is full we need to shift the array. The newest data goes to the end of the array.
            //If it is not full the data can be added to the first index where there is a null value.
            if(array[array.Length-1] != null)
            {
                for (int i = 0; i < array.Length - 1; i++)
                {
                    array[i] = array[i + 1];
                }
                array[array.Length - 1] = data;
            }
            else
            {
                int i = 0;
                while(array[i] != null)
                {
                    i++;
                }
                array[i] = data;
            }
        }

        //Button for manual chart update
        private void btUpdate_Click(object sender, EventArgs e)
        {
            UpdateCharts();
        }

        //Upon each tick (every 5 seconds) the charts are updated
        private void update_timer_Tick(object sender, EventArgs e)
        {
            UpdateCharts();
        }

        //Helper method to get the most recent data from the array
        private string GetLast(string[] array)
        {
            int i = array.Length - 1;
            while(array[i] == null)
            {
                i--;
            }
            return array[i];
        }

        //Tick method that gets the last temperature, humidity and als values and send them to the cloud. 
        private void trSendData_Tick(object sender, EventArgs e)
        {
            ConnectionUDP udp = new ConnectionUDP("867420041245814", "89882280666044066342");
            string t = GetLast(temperature).Replace(".", "");
            string h = GetLast(humidity).Replace(".", "");
            string a = GetLast(als);
            udp.SendData(t, h, a);
        }
    }
}