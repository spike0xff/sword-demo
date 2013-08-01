using System;
using System.Linq;
using System.Net;
using System.Text;
using System.Windows.Forms;

namespace Client
{
    public partial class FormSword : Form
    {
        public FormSword()
        {
            InitializeComponent();
        }

        private void m_buttonScan_Click(object sender, EventArgs e)
        {
            using (WebClient client = new WebClient())
            {
                this.m_richtextboxDiagnostics.Clear();
                System.Collections.Specialized.NameValueCollection reqparm = new System.Collections.Specialized.NameValueCollection();
                reqparm.Add
                (
                    "sword",
                    "<sword><scan></scan></sword>"
                );
                string values =
                    string.Join(", ", reqparm.AllKeys.Select(key => key + ": " + reqparm[key]).ToArray());
                    //string.Join(",", reqparm.AllKeys.SelectMany(key => reqparm.GetValues(key)));
                this.m_richtextboxDiagnostics.AppendText("POST: " + values + "\n");
                byte[] responsebytes = client.UploadValues("http://192.168.1.137:80", "POST", reqparm);
                string responsebody = Encoding.UTF8.GetString(responsebytes);
                this.m_richtextboxDiagnostics.AppendText("response: " + responsebody + "\n");
            }
        }
    }
}
