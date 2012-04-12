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
            this.toggleButton = new System.Windows.Forms.Button();
            this.usernameInput = new System.Windows.Forms.TextBox();
            this.testButton = new System.Windows.Forms.Button();
            this.toggleLabel = new System.Windows.Forms.Label();
            this.userLabel = new System.Windows.Forms.Label();
            this.testLabel = new System.Windows.Forms.Label();
            this.enableCheckBox = new System.Windows.Forms.CheckBox();
            this.confButton = new System.Windows.Forms.Button();
            this.testOutputLabel = new System.Windows.Forms.Label();
            this.safemodeCheckBox = new System.Windows.Forms.CheckBox();
            this.searchButton = new System.Windows.Forms.Button();
            this.iterationsInput = new System.Windows.Forms.TextBox();
            this.setIterationsButton = new System.Windows.Forms.Button();
            this.iterationsLabel = new System.Windows.Forms.Label();
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
            // usernameInput
            // 
            this.usernameInput.Location = new System.Drawing.Point(12, 154);
            this.usernameInput.Name = "usernameInput";
            this.usernameInput.Size = new System.Drawing.Size(152, 20);
            this.usernameInput.TabIndex = 1;
            this.usernameInput.KeyDown += new System.Windows.Forms.KeyEventHandler(this.username_KeyDown);
            // 
            // testButton
            // 
            this.testButton.Enabled = false;
            this.testButton.Location = new System.Drawing.Point(12, 236);
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
            // userLabel
            // 
            this.userLabel.AutoSize = true;
            this.userLabel.Location = new System.Drawing.Point(9, 138);
            this.userLabel.Name = "userLabel";
            this.userLabel.Size = new System.Drawing.Size(53, 13);
            this.userLabel.TabIndex = 4;
            this.userLabel.Text = "username";
            // 
            // testLabel
            // 
            this.testLabel.AutoSize = true;
            this.testLabel.Location = new System.Drawing.Point(9, 220);
            this.testLabel.Name = "testLabel";
            this.testLabel.Size = new System.Drawing.Size(64, 13);
            this.testLabel.TabIndex = 5;
            this.testLabel.Text = "press to test";
            // 
            // enableCheckBox
            // 
            this.enableCheckBox.AutoSize = true;
            this.enableCheckBox.Enabled = false;
            this.enableCheckBox.Location = new System.Drawing.Point(127, 184);
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
            this.confButton.Location = new System.Drawing.Point(12, 180);
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
            this.testOutputLabel.Location = new System.Drawing.Point(9, 262);
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
            // searchButton
            // 
            this.searchButton.Location = new System.Drawing.Point(170, 152);
            this.searchButton.Name = "searchButton";
            this.searchButton.Size = new System.Drawing.Size(47, 26);
            this.searchButton.TabIndex = 10;
            this.searchButton.Text = "search";
            this.searchButton.UseVisualStyleBackColor = true;
            this.searchButton.Click += new System.EventHandler(this.searchButton_Click);
            // 
            // iterationsInput
            // 
            this.iterationsInput.Location = new System.Drawing.Point(12, 103);
            this.iterationsInput.Name = "iterationsInput";
            this.iterationsInput.Size = new System.Drawing.Size(100, 20);
            this.iterationsInput.TabIndex = 11;
            this.iterationsInput.KeyDown += new System.Windows.Forms.KeyEventHandler(this.iterations_KeyDown);
            // 
            // setIterationsButton
            // 
            this.setIterationsButton.Location = new System.Drawing.Point(170, 101);
            this.setIterationsButton.Name = "setIterationsButton";
            this.setIterationsButton.Size = new System.Drawing.Size(31, 22);
            this.setIterationsButton.TabIndex = 12;
            this.setIterationsButton.Text = "set";
            this.setIterationsButton.UseVisualStyleBackColor = true;
            this.setIterationsButton.Click += new System.EventHandler(this.setIterations_Click);
            // 
            // iterationsLabel
            // 
            this.iterationsLabel.AutoSize = true;
            this.iterationsLabel.Location = new System.Drawing.Point(9, 87);
            this.iterationsLabel.Name = "iterationsLabel";
            this.iterationsLabel.Size = new System.Drawing.Size(78, 13);
            this.iterationsLabel.TabIndex = 13;
            this.iterationsLabel.Text = "Hash Iterations";
            // 
            // YubiSettings
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(267, 295);
            this.Controls.Add(this.iterationsLabel);
            this.Controls.Add(this.setIterationsButton);
            this.Controls.Add(this.iterationsInput);
            this.Controls.Add(this.searchButton);
            this.Controls.Add(this.safemodeCheckBox);
            this.Controls.Add(this.testOutputLabel);
            this.Controls.Add(this.confButton);
            this.Controls.Add(this.enableCheckBox);
            this.Controls.Add(this.testLabel);
            this.Controls.Add(this.userLabel);
            this.Controls.Add(this.toggleLabel);
            this.Controls.Add(this.testButton);
            this.Controls.Add(this.usernameInput);
            this.Controls.Add(this.toggleButton);
            this.Name = "YubiSettings";
            this.Text = "YubiKey Logon Administration";
            this.Load += new System.EventHandler(this.YubiSettings_Load);
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.Button toggleButton;
        private System.Windows.Forms.TextBox usernameInput;
        private System.Windows.Forms.Button testButton;
        private System.Windows.Forms.Label toggleLabel;
        private System.Windows.Forms.Label userLabel;
        private System.Windows.Forms.Label testLabel;
        private System.Windows.Forms.CheckBox enableCheckBox;
        private System.Windows.Forms.Button confButton;
        private System.Windows.Forms.Label testOutputLabel;
        private System.Windows.Forms.CheckBox safemodeCheckBox;
        private System.Windows.Forms.Button searchButton;
        private System.Windows.Forms.TextBox iterationsInput;
        private System.Windows.Forms.Button setIterationsButton;
        private System.Windows.Forms.Label iterationsLabel;
    }
}

