using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using System.IO.Ports;
namespace ECS_Final
{
	public partial class MainForm : Form
	{
		public MainForm()
		{
			InitializeComponent();
		}

		private void butPlus_Click(object sender, EventArgs e)
		{
			i++;
			Console.WriteLine(i);
		}

		private void butMinus_Click(object sender, EventArgs e)
		{
			i--;
			Console.WriteLine(i);
		}

		private void MainForm_Load(object sender, EventArgs e)
		{
			//mainPort.ReadBufferSize = 13;
			portUart.Open();
			timer1.Start();
		}

		private void portUart_DataReceived(object sender, SerialDataReceivedEventArgs e)
		{
			string tmp = portUart.ReadExisting();
			Console.WriteLine(tmp);
			string[] tmp2 = tmp.Split('\n')[0].Split('-');
			setPoint = Convert.ToDouble(tmp2[0]);
			curPos= Convert.ToDouble(tmp2[1]);
			useENC = (tmp2[2].ElementAt(0) - 0x30) == 1 ? true : false;
			//portUart.DiscardInBuffer();
		}

		private void timer1_Tick(object sender, EventArgs e)
		{
			time += 1;
			chartMotorPos.Series["Set point"].Points.AddXY(time, setPoint);
			chartMotorPos.Series["Current position"].Points.AddXY(time, curPos);

			if (chartMotorPos.Series["Set point"].Points.Count > 100)
			{
				chartMotorPos.Series["Set point"].Points.RemoveAt(0);
				chartMotorPos.Series["Current position"].Points.RemoveAt(0);

			}
			chartMotorPos.ChartAreas[0].RecalculateAxesScale();
			portUart.Write("r");
		}

		private void butSendCmd_Click(object sender, EventArgs e)
		{
			if (cbUseEnc.CheckState == CheckState.Checked != useENC)
			{
				portUart.Write("c");
			}
			else 
			{
				portUart.Write(tbSetPoint.Text);
			}
		}

        private void chartMotorPos_Click(object sender, EventArgs e)
        {

        }
    }
}
