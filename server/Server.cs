using System;
using System.Collections.Specialized;
using System.IO;
using System.Net;
using System.Web;

namespace Server
{
    public partial class SwordServer
    {
        private static System.Threading.AutoResetEvent listenForNextRequest = new System.Threading.AutoResetEvent(false);

        public SwordServer()
        {
            _httpListener = new HttpListener();
        }

        private HttpListener _httpListener;

        public string Prefix { get; set; }
        public void Start()
        {
            if (String.IsNullOrEmpty(Prefix))
                throw new InvalidOperationException("No prefix has been specified");
            _httpListener.Prefixes.Clear();
            _httpListener.Prefixes.Add(Prefix);
            _httpListener.Start();
            System.Threading.ThreadPool.QueueUserWorkItem(Listen);
        }

        internal void Stop()
        {
            _httpListener.Stop();
            IsRunning = false;
        }

        public bool IsRunning { get; private set; }

        private void ListenerCallback(IAsyncResult result)
        {
            HttpListener listener = result.AsyncState as HttpListener;
            HttpListenerContext context = null;

            if (listener == null)
                // Nevermind 
                return;

            try
            {
                context = listener.EndGetContext(result);
            }
            catch (Exception ex)
            {
                System.Diagnostics.Debug.WriteLine(ex.ToString());
                return;
            }
            finally
            {
                listenForNextRequest.Set();
            }
            if (context == null)
            {
                return;
            }
            ProcessRequest(context);
        }

        // Loop here to begin processing of new requests. 
        private void Listen(object state)
        {
            while (_httpListener.IsListening)
            {
                _httpListener.BeginGetContext(new AsyncCallback(ListenerCallback), _httpListener);
                listenForNextRequest.WaitOne();
            }
        }

        private void ProcessRequest(HttpListenerContext context)
        {
            string input = null;
            using (StreamReader reader = new StreamReader (context.Request.InputStream))
            {
                input = reader.ReadToEnd();
            }
            NameValueCollection coll = HttpUtility.ParseQueryString(input);
            string s = coll.GetValues("sword")[0];
            Console.Out.WriteLine(s); Console.Out.Flush();
            string r = "<sword><status>success</status></sword>";
            byte[] buffer = System.Text.Encoding.UTF8.GetBytes(r);
            context.Response.ContentLength64 = buffer.Length;
            System.IO.Stream output = context.Response.OutputStream;
            Console.Out.WriteLine("1..."); Console.Out.Flush(); 
            output.Write(buffer, 0, buffer.Length);
            Console.Out.WriteLine("2..."); Console.Out.Flush(); 
            output.Close();
            Console.Out.WriteLine("3..."); Console.Out.Flush(); 
            return;
        }
    }
}
