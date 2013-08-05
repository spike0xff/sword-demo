using System;
using System.Collections;
using System.Linq;
using System.Net;
using System.Text;
using System.Text.RegularExpressions;
using System.Windows.Forms;
using Mono.Zeroconf;
using System.ComponentModel;
using System.Threading;


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
                try
                {
                    byte[] responsebytes = client.UploadValues("http://" + this.scannerAddr.Text, "POST", reqparm);
                    string responsebody = Encoding.UTF8.GetString(responsebytes);
                    this.m_richtextboxDiagnostics.AppendText("response: " + responsebody + "\n");
                }
                catch (SystemException ex)
                {
                    this.m_richtextboxDiagnostics.AppendText("Exception: "+ex.ToString());
                }
            }
        }

        private SynchronizationContext syncContext;

        private static bool resolve_shares = true;
        private static uint @interface = 0;
        private static AddressProtocol address_protocol = AddressProtocol.Any;
        private static string domain = "local";

        private void FormSword_Load(object sender, EventArgs e)
        {
            syncContext = AsyncOperationManager.SynchronizationContext;
            string type = "_sword._tcp";
            try
            {
                // Listen for events of our service type
                ServiceBrowser browser = new ServiceBrowser();
                browser.ServiceAdded += OnServiceAdded;
                browser.ServiceRemoved += OnServiceRemoved;
                browser.Browse(@interface, address_protocol, type, domain);
            }
            catch (Exception x) {
                this.deviceList.Items.Add(x);
                deviceList.Enabled = false;
            }
        }

        private void OnServiceAdded(object o, ServiceBrowseEventArgs args)
        {
            if (resolve_shares)
            {
                args.Service.Resolved += OnServiceResolved;
                args.Service.Resolve();
            }
        }

        private void OnServiceRemoved(object o, ServiceBrowseEventArgs args)
        {
            String device = args.Service.Name;
            syncContext.Post(delegate
            {
                deviceList.Items.Remove(device);
            }, null);
        }

        private void OnServiceResolved(object o, ServiceResolvedEventArgs args)
        {
            IResolvableService service = o as IResolvableService;
            String device = args.Service.Name + " [" + service.HostEntry.AddressList[0] + "]";
            syncContext.Post(delegate
            {
                if (!deviceList.Items.Contains(device))
                {
                    deviceList.Items.Add(device);
                }
            }, null);

            ITxtRecord record = service.TxtRecord;
            int record_count = record != null ? record.Count : 0;
            if (record_count > 0)
            {
                Console.Write(", TXT Record = [");
                for (int i = 0, n = record.Count; i < n; i++)
                {
                    TxtRecordItem item = record.GetItemAt(i);
                    Console.Write("{0} = '{1}'", item.Key, item.ValueString);
                    if (i < n - 1)
                    {
                        Console.Write(", ");
                    }
                }
                Console.WriteLine("]");
            }
            else
            {
                Console.WriteLine("");
            }
        }

        private void deviceList_SelectedValueChanged(object sender, EventArgs e)
        {
            string device = this.deviceList.SelectedItems[0] as string;
            int i = device.LastIndexOf('[');
            int j = device.LastIndexOf(']');
            if (i > 0 && j > i)
            {
                scannerAddr.Text = device.Substring(i + 1, j - i - 1);
            }
            else
            {
                scannerAddr.Text = device;
            }
        }


    }
}
