using System.IO.Ports;

namespace ECS_Final
{
	partial class MainForm
	{
		/// <summary>
		/// Required designer variable.
		/// </summary>
		private System.ComponentModel.IContainer components = null;

		/// <summary>
		/// Clean up any resources being used.
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
		/// Required method for Designer support - do not modify
		/// the contents of this method with the code editor.
		/// </summary>
		private void InitializeComponent()
		{
            this.components = new System.ComponentModel.Container();
            System.Windows.Forms.DataVisualization.Charting.ChartArea chartArea1 = new System.Windows.Forms.DataVisualization.Charting.ChartArea();
            System.Windows.Forms.DataVisualization.Charting.Legend legend1 = new System.Windows.Forms.DataVisualization.Charting.Legend();
            System.Windows.Forms.DataVisualization.Charting.Series series1 = new System.Windows.Forms.DataVisualization.Charting.Series();
            System.Windows.Forms.DataVisualization.Charting.Series series2 = new System.Windows.Forms.DataVisualization.Charting.Series();
            this.chartMotorPos = new System.Windows.Forms.DataVisualization.Charting.Chart();
            this.butPlus = new System.Windows.Forms.Button();
            this.butMinus = new System.Windows.Forms.Button();
            this.lblSetPoint = new System.Windows.Forms.Label();
            this.timer1 = new System.Windows.Forms.Timer(this.components);
            this.gbUartCmd = new System.Windows.Forms.GroupBox();
            this.butSendCmd = new System.Windows.Forms.Button();
            this.tbSetPoint = new System.Windows.Forms.TextBox();
            this.cbUseEnc = new System.Windows.Forms.CheckBox();
            this.portUart = new System.IO.Ports.SerialPort(this.components);
            ((System.ComponentModel.ISupportInitialize)(this.chartMotorPos)).BeginInit();
            this.gbUartCmd.SuspendLayout();
            this.SuspendLayout();
            // 
            // chartMotorPos
            // 
            chartArea1.Name = "ChartArea1";
            this.chartMotorPos.ChartAreas.Add(chartArea1);
            legend1.Name = "Legend1";
            this.chartMotorPos.Legends.Add(legend1);
            this.chartMotorPos.Location = new System.Drawing.Point(16, 0);
            this.chartMotorPos.Margin = new System.Windows.Forms.Padding(4, 4, 4, 4);
            this.chartMotorPos.Name = "chartMotorPos";
            series1.BorderWidth = 4;
            series1.ChartArea = "ChartArea1";
            series1.ChartType = System.Windows.Forms.DataVisualization.Charting.SeriesChartType.FastLine;
            series1.IsXValueIndexed = true;
            series1.Legend = "Legend1";
            series1.Name = "Set point";
            series2.BorderWidth = 4;
            series2.ChartArea = "ChartArea1";
            series2.ChartType = System.Windows.Forms.DataVisualization.Charting.SeriesChartType.FastLine;
            series2.IsXValueIndexed = true;
            series2.Legend = "Legend1";
            series2.Name = "Current position";
            this.chartMotorPos.Series.Add(series1);
            this.chartMotorPos.Series.Add(series2);
            this.chartMotorPos.Size = new System.Drawing.Size(1425, 497);
            this.chartMotorPos.TabIndex = 0;
            this.chartMotorPos.Text = "Position display";
            this.chartMotorPos.Click += new System.EventHandler(this.chartMotorPos_Click);
            // 
            // butPlus
            // 
            this.butPlus.AccessibleName = "buttonPlus";
            this.butPlus.Location = new System.Drawing.Point(1067, 34);
            this.butPlus.Margin = new System.Windows.Forms.Padding(4, 4, 4, 4);
            this.butPlus.Name = "butPlus";
            this.butPlus.Size = new System.Drawing.Size(71, 53);
            this.butPlus.TabIndex = 1;
            this.butPlus.Text = "+";
            this.butPlus.UseVisualStyleBackColor = true;
            this.butPlus.Click += new System.EventHandler(this.butPlus_Click);
            // 
            // butMinus
            // 
            this.butMinus.AccessibleName = "buttonMinus";
            this.butMinus.Location = new System.Drawing.Point(1067, 95);
            this.butMinus.Margin = new System.Windows.Forms.Padding(4, 4, 4, 4);
            this.butMinus.Name = "butMinus";
            this.butMinus.Size = new System.Drawing.Size(71, 65);
            this.butMinus.TabIndex = 1;
            this.butMinus.Text = "-";
            this.butMinus.UseVisualStyleBackColor = true;
            this.butMinus.Click += new System.EventHandler(this.butMinus_Click);
            // 
            // lblSetPoint
            // 
            this.lblSetPoint.AccessibleName = "Set point label";
            this.lblSetPoint.AutoSize = true;
            this.lblSetPoint.Font = new System.Drawing.Font("Microsoft Sans Serif", 14.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.lblSetPoint.Location = new System.Drawing.Point(660, 82);
            this.lblSetPoint.Margin = new System.Windows.Forms.Padding(4, 0, 4, 0);
            this.lblSetPoint.Name = "lblSetPoint";
            this.lblSetPoint.Size = new System.Drawing.Size(210, 29);
            this.lblSetPoint.TabIndex = 2;
            this.lblSetPoint.Text = "Set point [0 .. 359]:";
            // 
            // timer1
            // 
            this.timer1.Interval = 200;
            this.timer1.Tick += new System.EventHandler(this.timer1_Tick);
            // 
            // gbUartCmd
            // 
            this.gbUartCmd.Controls.Add(this.butSendCmd);
            this.gbUartCmd.Controls.Add(this.tbSetPoint);
            this.gbUartCmd.Controls.Add(this.butMinus);
            this.gbUartCmd.Controls.Add(this.butPlus);
            this.gbUartCmd.Controls.Add(this.cbUseEnc);
            this.gbUartCmd.Controls.Add(this.lblSetPoint);
            this.gbUartCmd.Font = new System.Drawing.Font("Microsoft Sans Serif", 20.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.gbUartCmd.Location = new System.Drawing.Point(16, 505);
            this.gbUartCmd.Margin = new System.Windows.Forms.Padding(4, 4, 4, 4);
            this.gbUartCmd.Name = "gbUartCmd";
            this.gbUartCmd.Padding = new System.Windows.Forms.Padding(4, 4, 4, 4);
            this.gbUartCmd.Size = new System.Drawing.Size(1165, 335);
            this.gbUartCmd.TabIndex = 4;
            this.gbUartCmd.TabStop = false;
            this.gbUartCmd.Text = "UART command";
            // 
            // butSendCmd
            // 
            this.butSendCmd.Location = new System.Drawing.Point(844, 251);
            this.butSendCmd.Margin = new System.Windows.Forms.Padding(4, 4, 4, 4);
            this.butSendCmd.Name = "butSendCmd";
            this.butSendCmd.Size = new System.Drawing.Size(293, 65);
            this.butSendCmd.TabIndex = 5;
            this.butSendCmd.Text = "Send command";
            this.butSendCmd.UseVisualStyleBackColor = true;
            this.butSendCmd.Click += new System.EventHandler(this.butSendCmd_Click);
            // 
            // tbSetPoint
            // 
            this.tbSetPoint.BackColor = System.Drawing.SystemColors.GradientActiveCaption;
            this.tbSetPoint.Font = new System.Drawing.Font("Microsoft Sans Serif", 14.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.tbSetPoint.Location = new System.Drawing.Point(873, 76);
            this.tbSetPoint.Margin = new System.Windows.Forms.Padding(4, 4, 4, 4);
            this.tbSetPoint.MaxLength = 3;
            this.tbSetPoint.Name = "tbSetPoint";
            this.tbSetPoint.Size = new System.Drawing.Size(172, 34);
            this.tbSetPoint.TabIndex = 4;
            this.tbSetPoint.Text = "180";
            // 
            // cbUseEnc
            // 
            this.cbUseEnc.AutoSize = true;
            this.cbUseEnc.Font = new System.Drawing.Font("Microsoft Sans Serif", 14.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.cbUseEnc.Location = new System.Drawing.Point(109, 53);
            this.cbUseEnc.Margin = new System.Windows.Forms.Padding(4, 4, 4, 4);
            this.cbUseEnc.Name = "cbUseEnc";
            this.cbUseEnc.Size = new System.Drawing.Size(181, 33);
            this.cbUseEnc.TabIndex = 3;
            this.cbUseEnc.Text = " Use Encoder";
            this.cbUseEnc.UseVisualStyleBackColor = true;
            // 
            // portUart
            // 
            this.portUart.PortName = "COM5";
            this.portUart.DataReceived += new System.IO.Ports.SerialDataReceivedEventHandler(this.portUart_DataReceived);
            // 
            // MainForm
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(8F, 16F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(1457, 854);
            this.Controls.Add(this.gbUartCmd);
            this.Controls.Add(this.chartMotorPos);
            this.Margin = new System.Windows.Forms.Padding(4, 4, 4, 4);
            this.Name = "MainForm";
            this.Text = "STM32 DC motor position control";
            this.Load += new System.EventHandler(this.MainForm_Load);
            ((System.ComponentModel.ISupportInitialize)(this.chartMotorPos)).EndInit();
            this.gbUartCmd.ResumeLayout(false);
            this.gbUartCmd.PerformLayout();
            this.ResumeLayout(false);

		}

		#endregion

		private System.Windows.Forms.DataVisualization.Charting.Chart chartMotorPos;
		private System.Windows.Forms.Button butPlus;
		private System.Windows.Forms.Button butMinus;
		private System.Windows.Forms.Label lblSetPoint;
		private System.Windows.Forms.Timer timer1;
		private System.Windows.Forms.GroupBox gbUartCmd;
		private System.Windows.Forms.Button butSendCmd;
		private System.Windows.Forms.TextBox tbSetPoint;
		private System.Windows.Forms.CheckBox cbUseEnc;
		private int i;
		private double setPoint;
		private double curPos;
		private bool useENC;
		public int time;
		private SerialPort portUart;
	}
}

