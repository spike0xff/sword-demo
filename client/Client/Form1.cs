using System;
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
                System.Collections.Specialized.NameValueCollection reqparm = new System.Collections.Specialized.NameValueCollection();
                reqparm.Add
                (
                    "sword",
                    "<sword><scan></scan></sword>"
                );
                byte[] responsebytes = client.UploadValues("http://172.16.2.13:12345", "POST", reqparm);
                string responsebody = Encoding.UTF8.GetString(responsebytes);
            }
        }
    }
}
