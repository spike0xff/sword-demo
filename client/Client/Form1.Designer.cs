﻿namespace Client
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
            this.label1 = new System.Windows.Forms.Label();
            this.scannerAddr = new System.Windows.Forms.TextBox();
            this.deviceList = new System.Windows.Forms.ListBox();
            this.label2 = new System.Windows.Forms.Label();
            this.pictureBox1 = new System.Windows.Forms.PictureBox();
            this.label3 = new System.Windows.Forms.Label();
            ((System.ComponentModel.ISupportInitialize)(this.pictureBox1)).BeginInit();
            this.SuspendLayout();
            // 
            // m_buttonScan
            // 
            this.m_buttonScan.Location = new System.Drawing.Point(15, 159);
            this.m_buttonScan.Name = "m_buttonScan";
            this.m_buttonScan.Size = new System.Drawing.Size(79, 27);
            this.m_buttonScan.TabIndex = 5;
            this.m_buttonScan.Text = "Scan";
            this.m_buttonScan.UseVisualStyleBackColor = true;
            this.m_buttonScan.Click += new System.EventHandler(this.m_buttonScan_Click);
            // 
            // m_buttonStop
            // 
            this.m_buttonStop.Enabled = false;
            this.m_buttonStop.Location = new System.Drawing.Point(117, 159);
            this.m_buttonStop.Name = "m_buttonStop";
            this.m_buttonStop.Size = new System.Drawing.Size(79, 27);
            this.m_buttonStop.TabIndex = 6;
            this.m_buttonStop.Text = "Stop";
            this.m_buttonStop.UseVisualStyleBackColor = true;
            // 
            // m_comboboxColor
            // 
            this.m_comboboxColor.FormattingEnabled = true;
            this.m_comboboxColor.Items.AddRange(new object[] {
            "BW (1-bit) --crashes the pi!",
            "Grayscale (8-bit)",
            "Full Color (24-bit)"});
            this.m_comboboxColor.Location = new System.Drawing.Point(80, 77);
            this.m_comboboxColor.Name = "m_comboboxColor";
            this.m_comboboxColor.Size = new System.Drawing.Size(121, 21);
            this.m_comboboxColor.TabIndex = 2;
            // 
            // m_labelColor
            // 
            this.m_labelColor.AutoSize = true;
            this.m_labelColor.Location = new System.Drawing.Point(12, 80);
            this.m_labelColor.Name = "m_labelColor";
            this.m_labelColor.Size = new System.Drawing.Size(34, 13);
            this.m_labelColor.TabIndex = 3;
            this.m_labelColor.Text = "Color:";
            // 
            // m_labelResolution
            // 
            this.m_labelResolution.AutoSize = true;
            this.m_labelResolution.Location = new System.Drawing.Point(12, 108);
            this.m_labelResolution.Name = "m_labelResolution";
            this.m_labelResolution.Size = new System.Drawing.Size(60, 13);
            this.m_labelResolution.TabIndex = 4;
            this.m_labelResolution.Text = "Resolution:";
            // 
            // m_comboxBoxResolution
            // 
            this.m_comboxBoxResolution.FormattingEnabled = true;
            this.m_comboxBoxResolution.Items.AddRange(new object[] {
            "75",
            "100",
            "150",
            "200",
            "300",
            "600"});
            this.m_comboxBoxResolution.Location = new System.Drawing.Point(80, 105);
            this.m_comboxBoxResolution.Name = "m_comboxBoxResolution";
            this.m_comboxBoxResolution.Size = new System.Drawing.Size(121, 21);
            this.m_comboxBoxResolution.TabIndex = 3;
            // 
            // m_richtextboxDiagnostics
            // 
            this.m_richtextboxDiagnostics.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom)
                        | System.Windows.Forms.AnchorStyles.Left)));
            this.m_richtextboxDiagnostics.Location = new System.Drawing.Point(15, 221);
            this.m_richtextboxDiagnostics.Name = "m_richtextboxDiagnostics";
            this.m_richtextboxDiagnostics.Size = new System.Drawing.Size(328, 283);
            this.m_richtextboxDiagnostics.TabIndex = 4;
            this.m_richtextboxDiagnostics.Text = "";
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(12, 9);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(50, 13);
            this.label1.TabIndex = 7;
            this.label1.Text = "Scanner:";
            // 
            // scannerAddr
            // 
            this.scannerAddr.ForeColor = System.Drawing.SystemColors.ControlDark;
            this.scannerAddr.Location = new System.Drawing.Point(428, 7);
            this.scannerAddr.Name = "scannerAddr";
            this.scannerAddr.Size = new System.Drawing.Size(158, 20);
            this.scannerAddr.TabIndex = 1;
            this.scannerAddr.Text = "192.168.1.137";
            // 
            // deviceList
            // 
            this.deviceList.FormattingEnabled = true;
            this.deviceList.Location = new System.Drawing.Point(80, 7);
            this.deviceList.Name = "deviceList";
            this.deviceList.Size = new System.Drawing.Size(275, 56);
            this.deviceList.TabIndex = 8;
            this.deviceList.SelectedValueChanged += new System.EventHandler(this.deviceList_SelectedValueChanged);
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Location = new System.Drawing.Point(372, 10);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(48, 13);
            this.label2.TabIndex = 7;
            this.label2.Text = "Address:";
            // 
            // pictureBox1
            // 
            this.pictureBox1.Location = new System.Drawing.Point(15, 140);
            this.pictureBox1.Name = "pictureBox1";
            this.pictureBox1.Size = new System.Drawing.Size(181, 1);
            this.pictureBox1.TabIndex = 9;
            this.pictureBox1.TabStop = false;
            // 
            // label3
            // 
            this.label3.AutoSize = true;
            this.label3.Location = new System.Drawing.Point(12, 204);
            this.label3.Name = "label3";
            this.label3.Size = new System.Drawing.Size(28, 13);
            this.label3.TabIndex = 10;
            this.label3.Text = "Log:";
            // 
            // FormSword
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.BackColor = System.Drawing.Color.LightGray;
            this.ClientSize = new System.Drawing.Size(647, 516);
            this.Controls.Add(this.label3);
            this.Controls.Add(this.pictureBox1);
            this.Controls.Add(this.deviceList);
            this.Controls.Add(this.scannerAddr);
            this.Controls.Add(this.label2);
            this.Controls.Add(this.label1);
            this.Controls.Add(this.m_richtextboxDiagnostics);
            this.Controls.Add(this.m_comboxBoxResolution);
            this.Controls.Add(this.m_labelResolution);
            this.Controls.Add(this.m_labelColor);
            this.Controls.Add(this.m_comboboxColor);
            this.Controls.Add(this.m_buttonStop);
            this.Controls.Add(this.m_buttonScan);
            this.Name = "FormSword";
            this.Text = "SWORD Demo";
            this.Load += new System.EventHandler(this.FormSword_Load);
            ((System.ComponentModel.ISupportInitialize)(this.pictureBox1)).EndInit();
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
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.TextBox scannerAddr;
        private System.Windows.Forms.ListBox deviceList;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.PictureBox pictureBox1;
        private System.Windows.Forms.Label label3;
    }
}

