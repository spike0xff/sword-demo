namespace Client
{
    partial class FormSword
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
            this.m_buttonScan = new System.Windows.Forms.Button();
            this.m_buttonStop = new System.Windows.Forms.Button();
            this.m_comboboxColor = new System.Windows.Forms.ComboBox();
            this.m_labelColor = new System.Windows.Forms.Label();
            this.m_labelResolution = new System.Windows.Forms.Label();
            this.m_comboxBoxResolution = new System.Windows.Forms.ComboBox();
            this.m_richtextboxDiagnostics = new System.Windows.Forms.RichTextBox();
            this.SuspendLayout();
            // 
            // m_buttonScan
            // 
            this.m_buttonScan.Location = new System.Drawing.Point(458, 248);
            this.m_buttonScan.Name = "m_buttonScan";
            this.m_buttonScan.Size = new System.Drawing.Size(75, 23);
            this.m_buttonScan.TabIndex = 0;
            this.m_buttonScan.Text = "Scan";
            this.m_buttonScan.UseVisualStyleBackColor = true;
            this.m_buttonScan.Click += new System.EventHandler(this.m_buttonScan_Click);
            // 
            // m_buttonStop
            // 
            this.m_buttonStop.Location = new System.Drawing.Point(539, 248);
            this.m_buttonStop.Name = "m_buttonStop";
            this.m_buttonStop.Size = new System.Drawing.Size(75, 23);
            this.m_buttonStop.TabIndex = 1;
            this.m_buttonStop.Text = "Stop";
            this.m_buttonStop.UseVisualStyleBackColor = true;
            // 
            // m_comboboxColor
            // 
            this.m_comboboxColor.FormattingEnabled = true;
            this.m_comboboxColor.Items.AddRange(new object[] {
            "Grayscale (8-bit)",
            "Full Color (24-bit)"});
            this.m_comboboxColor.Location = new System.Drawing.Point(101, 15);
            this.m_comboboxColor.Name = "m_comboboxColor";
            this.m_comboboxColor.Size = new System.Drawing.Size(121, 21);
            this.m_comboboxColor.TabIndex = 2;
            // 
            // m_labelColor
            // 
            this.m_labelColor.AutoSize = true;
            this.m_labelColor.Location = new System.Drawing.Point(13, 18);
            this.m_labelColor.Name = "m_labelColor";
            this.m_labelColor.Size = new System.Drawing.Size(34, 13);
            this.m_labelColor.TabIndex = 3;
            this.m_labelColor.Text = "Color:";
            // 
            // m_labelResolution
            // 
            this.m_labelResolution.AutoSize = true;
            this.m_labelResolution.Location = new System.Drawing.Point(12, 47);
            this.m_labelResolution.Name = "m_labelResolution";
            this.m_labelResolution.Size = new System.Drawing.Size(60, 13);
            this.m_labelResolution.TabIndex = 4;
            this.m_labelResolution.Text = "Resolution:";
            // 
            // m_comboxBoxResolution
            // 
            this.m_comboxBoxResolution.FormattingEnabled = true;
            this.m_comboxBoxResolution.Items.AddRange(new object[] {
            "200",
            "300"});
            this.m_comboxBoxResolution.Location = new System.Drawing.Point(101, 44);
            this.m_comboxBoxResolution.Name = "m_comboxBoxResolution";
            this.m_comboxBoxResolution.Size = new System.Drawing.Size(121, 21);
            this.m_comboxBoxResolution.TabIndex = 5;
            // 
            // m_richtextboxDiagnostics
            // 
            this.m_richtextboxDiagnostics.Location = new System.Drawing.Point(16, 82);
            this.m_richtextboxDiagnostics.Name = "m_richtextboxDiagnostics";
            this.m_richtextboxDiagnostics.Size = new System.Drawing.Size(598, 146);
            this.m_richtextboxDiagnostics.TabIndex = 6;
            this.m_richtextboxDiagnostics.Text = "";
            // 
            // FormSword
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.BackColor = System.Drawing.Color.LightGray;
            this.ClientSize = new System.Drawing.Size(626, 283);
            this.Controls.Add(this.m_richtextboxDiagnostics);
            this.Controls.Add(this.m_comboxBoxResolution);
            this.Controls.Add(this.m_labelResolution);
            this.Controls.Add(this.m_labelColor);
            this.Controls.Add(this.m_comboboxColor);
            this.Controls.Add(this.m_buttonStop);
            this.Controls.Add(this.m_buttonScan);
            this.Name = "FormSword";
            this.Text = "SWORD Demo";
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.Button m_buttonScan;
        private System.Windows.Forms.Button m_buttonStop;
        private System.Windows.Forms.ComboBox m_comboboxColor;
        private System.Windows.Forms.Label m_labelColor;
        private System.Windows.Forms.Label m_labelResolution;
        private System.Windows.Forms.ComboBox m_comboxBoxResolution;
        private System.Windows.Forms.RichTextBox m_richtextboxDiagnostics;
    }
}

