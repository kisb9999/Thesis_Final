namespace WindowsDemoApp
{
    partial class DemoApp
    {
        /// <summary>
        ///  Required designer variable.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        ///  Clean up any resources being used.
        /// </summary>
        /// <param name="disposing">true if managed resources should be disposed; otherwise, false.</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Windows Form Designer generated code

        /// <summary>
        ///  Required method for Designer support - do not modify
        ///  the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
            this.components = new System.ComponentModel.Container();
            System.Windows.Forms.DataVisualization.Charting.ChartArea chartArea1 = new System.Windows.Forms.DataVisualization.Charting.ChartArea();
            System.Windows.Forms.DataVisualization.Charting.Legend legend1 = new System.Windows.Forms.DataVisualization.Charting.Legend();
            System.Windows.Forms.DataVisualization.Charting.Series series1 = new System.Windows.Forms.DataVisualization.Charting.Series();
            System.Windows.Forms.DataVisualization.Charting.Series series2 = new System.Windows.Forms.DataVisualization.Charting.Series();
            System.Windows.Forms.DataVisualization.Charting.Series series3 = new System.Windows.Forms.DataVisualization.Charting.Series();
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(DemoApp));
            this.label1 = new System.Windows.Forms.Label();
            this.listView1 = new System.Windows.Forms.ListView();
            this.NeoData = new System.Windows.Forms.ColumnHeader();
            this.ch_test = new System.Windows.Forms.DataVisualization.Charting.Chart();
            this.update_timer = new System.Windows.Forms.Timer(this.components);
            this.btUpdate = new System.Windows.Forms.Button();
            this.trSendData = new System.Windows.Forms.Timer(this.components);
            ((System.ComponentModel.ISupportInitialize)(this.ch_test)).BeginInit();
            this.SuspendLayout();
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.BackColor = System.Drawing.Color.Transparent;
            this.label1.Font = new System.Drawing.Font("Segoe UI", 20.25F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point);
            this.label1.ForeColor = System.Drawing.SystemColors.Desktop;
            this.label1.Location = new System.Drawing.Point(12, 31);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(323, 37);
            this.label1.TabIndex = 1;
            this.label1.Text = "Data from the Gateway:";
            // 
            // listView1
            // 
            this.listView1.AllowDrop = true;
            this.listView1.BackColor = System.Drawing.SystemColors.ActiveCaptionText;
            this.listView1.Columns.AddRange(new System.Windows.Forms.ColumnHeader[] {
            this.NeoData});
            this.listView1.ForeColor = System.Drawing.SystemColors.Control;
            this.listView1.HeaderStyle = System.Windows.Forms.ColumnHeaderStyle.None;
            this.listView1.Location = new System.Drawing.Point(12, 71);
            this.listView1.Name = "listView1";
            this.listView1.Size = new System.Drawing.Size(975, 116);
            this.listView1.TabIndex = 2;
            this.listView1.UseCompatibleStateImageBehavior = false;
            this.listView1.View = System.Windows.Forms.View.Details;
            // 
            // NeoData
            // 
            this.NeoData.Text = "NeoData";
            this.NeoData.Width = 800;
            // 
            // ch_test
            // 
            this.ch_test.BackColor = System.Drawing.Color.Transparent;
            this.ch_test.BorderlineColor = System.Drawing.Color.Black;
            this.ch_test.BorderlineWidth = 10;
            chartArea1.Name = "ChartArea1";
            this.ch_test.ChartAreas.Add(chartArea1);
            legend1.Name = "Legend1";
            this.ch_test.Legends.Add(legend1);
            this.ch_test.Location = new System.Drawing.Point(12, 193);
            this.ch_test.Name = "ch_test";
            this.ch_test.Palette = System.Windows.Forms.DataVisualization.Charting.ChartColorPalette.EarthTones;
            series1.BackSecondaryColor = System.Drawing.Color.Transparent;
            series1.BorderColor = System.Drawing.Color.Black;
            series1.BorderWidth = 2;
            series1.ChartArea = "ChartArea1";
            series1.Color = System.Drawing.Color.Red;
            series1.Font = new System.Drawing.Font("Microsoft Sans Serif", 12F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point);
            series1.LabelBackColor = System.Drawing.Color.Black;
            series1.LabelBorderColor = System.Drawing.Color.Black;
            series1.LabelBorderWidth = 2;
            series1.Legend = "Legend1";
            series1.Name = "Temperature";
            series2.BorderColor = System.Drawing.Color.Black;
            series2.BorderWidth = 2;
            series2.ChartArea = "ChartArea1";
            series2.Legend = "Legend1";
            series2.Name = "Humidity";
            series3.BorderColor = System.Drawing.Color.Black;
            series3.BorderWidth = 2;
            series3.ChartArea = "ChartArea1";
            series3.Legend = "Legend1";
            series3.Name = "ALS";
            this.ch_test.Series.Add(series1);
            this.ch_test.Series.Add(series2);
            this.ch_test.Series.Add(series3);
            this.ch_test.Size = new System.Drawing.Size(975, 388);
            this.ch_test.TabIndex = 3;
            this.ch_test.Text = "Chart";
            // 
            // update_timer
            // 
            this.update_timer.Enabled = true;
            this.update_timer.Interval = 5000;
            this.update_timer.Tick += new System.EventHandler(this.update_timer_Tick);
            // 
            // btUpdate
            // 
            this.btUpdate.BackColor = System.Drawing.SystemColors.Highlight;
            this.btUpdate.Font = new System.Drawing.Font("Segoe UI", 14.25F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point);
            this.btUpdate.Location = new System.Drawing.Point(878, 509);
            this.btUpdate.Name = "btUpdate";
            this.btUpdate.Size = new System.Drawing.Size(109, 72);
            this.btUpdate.TabIndex = 4;
            this.btUpdate.Text = "Update";
            this.btUpdate.UseVisualStyleBackColor = false;
            this.btUpdate.Click += new System.EventHandler(this.btUpdate_Click);
            // 
            // trSendData
            // 
            this.trSendData.Interval = 30000;
            this.trSendData.Tick += new System.EventHandler(this.trSendData_Tick);
            // 
            // DemoApp
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(7F, 15F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.BackColor = System.Drawing.SystemColors.ActiveCaption;
            this.BackgroundImage = ((System.Drawing.Image)(resources.GetObject("$this.BackgroundImage")));
            this.ClientSize = new System.Drawing.Size(999, 659);
            this.Controls.Add(this.btUpdate);
            this.Controls.Add(this.ch_test);
            this.Controls.Add(this.listView1);
            this.Controls.Add(this.label1);
            this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedSingle;
            this.Name = "DemoApp";
            this.Text = "DemoApp";
            this.Load += new System.EventHandler(this.DemoApp_Load);
            ((System.ComponentModel.ISupportInitialize)(this.ch_test)).EndInit();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion
        private Label label1;
        private ListView listView1;
        private EventHandler label1_Click;
        private ColumnHeader NeoData;
        private System.Windows.Forms.DataVisualization.Charting.Chart ch_test;
        private System.Windows.Forms.Timer update_timer;
        private Button btUpdate;
        private System.Windows.Forms.Timer trSendData;
    }
}