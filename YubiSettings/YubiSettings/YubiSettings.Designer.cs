namespace YubiSettings
{
    partial class YubiSettings
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
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(YubiSettings));
            this.toggleButton = new System.Windows.Forms.Button();
            this.testButton = new System.Windows.Forms.Button();
            this.toggleLabel = new System.Windows.Forms.Label();
            this.testLabel = new System.Windows.Forms.Label();
            this.enableCheckBox = new System.Windows.Forms.CheckBox();
            this.confButton = new System.Windows.Forms.Button();
            this.testOutputLabel = new System.Windows.Forms.Label();
            this.safemodeCheckBox = new System.Windows.Forms.CheckBox();
            this.userSelect = new System.Windows.Forms.ComboBox();
            this.SuspendLayout();
            // 
            // toggleButton
            // 
            this.toggleButton.Location = new System.Drawing.Point(12, 45);
            this.toggleButton.Name = "toggleButton";
            this.toggleButton.Size = new System.Drawing.Size(75, 23);
            this.toggleButton.TabIndex = 0;
            this.toggleButton.Text = "toggle";
            this.toggleButton.UseVisualStyleBackColor = true;
            this.toggleButton.Click += new System.EventHandler(this.toggleButton_Click);
            // 
            // testButton
            // 
            this.testButton.Enabled = false;
            this.testButton.Location = new System.Drawing.Point(12, 184);
            this.testButton.Name = "testButton";
            this.testButton.Size = new System.Drawing.Size(75, 23);
            this.testButton.TabIndex = 2;
            this.testButton.Text = "test";
            this.testButton.UseVisualStyleBackColor = true;
            this.testButton.Click += new System.EventHandler(this.testButton_Click);
            // 
            // toggleLabel
            // 
            this.toggleLabel.AutoSize = true;
            this.toggleLabel.Location = new System.Drawing.Point(12, 19);
            this.toggleLabel.Name = "toggleLabel";
            this.toggleLabel.Size = new System.Drawing.Size(35, 13);
            this.toggleLabel.TabIndex = 3;
            this.toggleLabel.Text = "label1";
            // 
            // testLabel
            // 
            this.testLabel.AutoSize = true;
            this.testLabel.Location = new System.Drawing.Point(9, 168);
            this.testLabel.Name = "testLabel";
            this.testLabel.Size = new System.Drawing.Size(64, 13);
            this.testLabel.TabIndex = 5;
            this.testLabel.Text = "press to test";
            // 
            // enableCheckBox
            // 
            this.enableCheckBox.AutoSize = true;
            this.enableCheckBox.Enabled = false;
            this.enableCheckBox.Location = new System.Drawing.Point(127, 132);
            this.enableCheckBox.Name = "enableCheckBox";
            this.enableCheckBox.Size = new System.Drawing.Size(64, 17);
            this.enableCheckBox.TabIndex = 6;
            this.enableCheckBox.Text = "enabled";
            this.enableCheckBox.UseVisualStyleBackColor = true;
            this.enableCheckBox.Click += new System.EventHandler(this.toggleEnabled);
            // 
            // confButton
            // 
            this.confButton.Enabled = false;
            this.confButton.Location = new System.Drawing.Point(12, 128);
            this.confButton.Name = "confButton";
            this.confButton.Size = new System.Drawing.Size(75, 23);
            this.confButton.TabIndex = 7;
            this.confButton.Text = "configure";
            this.confButton.UseVisualStyleBackColor = true;
            this.confButton.Click += new System.EventHandler(this.configureButton_Click);
            // 
            // testOutputLabel
            // 
            this.testOutputLabel.AutoSize = true;
            this.testOutputLabel.Location = new System.Drawing.Point(9, 210);
            this.testOutputLabel.Name = "testOutputLabel";
            this.testOutputLabel.Size = new System.Drawing.Size(56, 13);
            this.testOutputLabel.TabIndex = 8;
            this.testOutputLabel.Text = "testOutput";
            // 
            // safemodeCheckBox
            // 
            this.safemodeCheckBox.AutoSize = true;
            this.safemodeCheckBox.Location = new System.Drawing.Point(127, 49);
            this.safemodeCheckBox.Name = "safemodeCheckBox";
            this.safemodeCheckBox.Size = new System.Drawing.Size(124, 17);
            this.safemodeCheckBox.TabIndex = 9;
            this.safemodeCheckBox.Text = "enabled in safemode";
            this.safemodeCheckBox.UseVisualStyleBackColor = true;
            this.safemodeCheckBox.Click += new System.EventHandler(this.toggleSafeMode);
            // 
            // userSelect
            // 
            this.userSelect.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.userSelect.FormattingEnabled = true;
            this.userSelect.Location = new System.Drawing.Point(12, 101);
            this.userSelect.Name = "userSelect";
            this.userSelect.Size = new System.Drawing.Size(100, 21);
            this.userSelect.TabIndex = 14;
            this.userSelect.Tag = "";
            this.userSelect.SelectedIndexChanged += new System.EventHandler(this.userSelected);
            // 
            // YubiSettings
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(271, 272);
            this.Controls.Add(this.userSelect);
            this.Controls.Add(this.safemodeCheckBox);
            this.Controls.Add(this.testOutputLabel);
            this.Controls.Add(this.confButton);
            this.Controls.Add(this.enableCheckBox);
            this.Controls.Add(this.testLabel);
            this.Controls.Add(this.toggleLabel);
            this.Controls.Add(this.testButton);
            this.Controls.Add(this.toggleButton);
            this.Icon = ((System.Drawing.Icon)(resources.GetObject("$this.Icon")));
            this.Name = "YubiSettings";
            this.Text = "YubiKey Logon Administration";
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.Button toggleButton;
        private System.Windows.Forms.Button testButton;
        private System.Windows.Forms.Label toggleLabel;
        private System.Windows.Forms.Label testLabel;
        private System.Windows.Forms.CheckBox enableCheckBox;
        private System.Windows.Forms.Button confButton;
        private System.Windows.Forms.Label testOutputLabel;
        private System.Windows.Forms.CheckBox safemodeCheckBox;
        private System.Windows.Forms.ComboBox userSelect;
    }
}

