using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using Microsoft.Win32;
using System.Management;
using System.DirectoryServices;
using YubiClientAPILib;
using System.Security.Cryptography;
using System.Diagnostics;

namespace YubiSettings
{
    public partial class YubiSettings : Form
    {
        private static String MSV1_0 = "SYSTEM\\CurrentControlSet\\Control\\LSA\\MSV1_0";
        private static String SETTINGS = "SOFTWARE\\Yubico\\auth\\settings";
        private static String SAFEMODE = "safemodeEnabled";
        private static String USERS = "SOFTWARE\\Yubico\\auth\\users\\";
        private static String CREDENTIAL_PROVIDERS = "SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Authentication\\Credential Providers\\";
        private static String CREDENTIAL_FILTERS = "SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Authentication\\Credential Provider Filters\\";
        private static String GUID = "{0f33b914-4f18-4824-8880-29bbe2e05179}";
        private static int DEFAULT_ITERATIONS = 50000;

        YubiClient api;

        delegate void setEnabledCallback(bool enabled);

        public YubiSettings()
        {
            InitializeComponent();

            try
            {
                api = new YubiClient();
            }
            catch
            {
                MessageBox.Show("Could not instantiate Yubico com api, is it installed?", "Yubico API failure", MessageBoxButtons.OK, MessageBoxIcon.Error);
                Application.Exit();
            }

            RegistryKey key = Registry.LocalMachine.OpenSubKey(MSV1_0);
            Object value = key.GetValue("Auth0");
            if (value == null)
            {
                toggleLabel.Text = "YubiAuth disabled";
            }
            else if (((string)value) == "msvsubauth")
            {
                toggleLabel.Text = "YubiAuth enabled";
            }
            else
            {
                toggleLabel.Text = "Unknown state";
            }

            RegistryKey settings = Registry.LocalMachine.CreateSubKey(SETTINGS);
            Object safemodeEnabled = settings.GetValue(SAFEMODE);
            if (safemodeEnabled != null && (int)safemodeEnabled == 1)
            {
                safemodeCheckBox.Checked = true;
            }
            object iterations = settings.GetValue("hashIterations");
            if (iterations == null)
            {
                iterations = DEFAULT_ITERATIONS;
            }
            iterationsInput.Text = iterations.ToString();

            testOutputLabel.Text = "";

            WqlObjectQuery query = new WqlObjectQuery("Select Name from WIN32_UserAccount where Disabled = false");
            ManagementObjectSearcher searcher = new ManagementObjectSearcher(query);
            foreach(ManagementBaseObject user in searcher.Get()) {
                userSelect.Items.Add(user.GetPropertyValue("Name"));
            }

            api.deviceInserted += new _IYubiClientEvents_deviceInsertedEventHandler(yubiKey_Inserted);
            api.deviceRemoved += new _IYubiClientEvents_deviceRemovedEventHandler(yubiKey_Removed);
        }

        private void toggleButton_Click(object sender, EventArgs e)
        {
            RegistryKey key = Registry.LocalMachine.CreateSubKey(MSV1_0);
            RegistryKey cKey = Registry.LocalMachine.CreateSubKey(CREDENTIAL_PROVIDERS);
            RegistryKey fKey = Registry.LocalMachine.CreateSubKey(CREDENTIAL_FILTERS);
            RegistryKey clsKey = Registry.ClassesRoot.CreateSubKey("CLSID\\" + GUID);
            Object value = key.GetValue("Auth0");
            if (value == null)
            {
                cKey = cKey.CreateSubKey(GUID);
                fKey = fKey.CreateSubKey(GUID);
                key.SetValue("Auth0", "msvsubauth");
                cKey.SetValue(null, "SampleWrapExistingCredentialProvider");
                fKey.SetValue(null, "SampleWrapExistingCredentialProvider");
                clsKey.SetValue(null, "SampleWrapExistingCredentialProvider");
                clsKey = clsKey.CreateSubKey("InprocServer32");
                clsKey.SetValue(null, "SampleWrapExistingCredentialProvider");
                clsKey.SetValue("ThreadingModel", "Apartment");
                toggleLabel.Text = "YubiAuth enabled";
            }
            else
            {
                key.DeleteValue("Auth0");
                cKey.DeleteSubKeyTree(GUID, false);
                fKey.DeleteSubKeyTree(GUID, false);
                Registry.ClassesRoot.CreateSubKey("CLSID").DeleteSubKeyTree(GUID, false);
                toggleLabel.Text = "YubiAuth disabled";
            }
        }

        private void toggleSafeMode(object sender, EventArgs e)
        {
            RegistryKey key = Registry.LocalMachine.CreateSubKey(SETTINGS);
            CheckBox checkbox = (CheckBox)sender;
            if(checkbox.Checked) {
                key.SetValue(SAFEMODE, 1);
            } else {
                key.SetValue(SAFEMODE, 0);
            }
        }

        private void userSelected(object sender, EventArgs e)
        {
            String username = userSelect.SelectedItem.ToString();
            RegistryKey userKey = Registry.LocalMachine.CreateSubKey(USERS + username);
            object enabled = userKey.GetValue("enabled");
            if (enabled == null || (int)enabled != 1)
            {
                enableCheckBox.Checked = false;
            }
            else
            {
                enableCheckBox.Checked = true;
            }
            enableCheckBox.Enabled = true;
            api.enableNotifications = ycNOTIFICATION_MODE.ycNOTIFICATION_ON;
        }

        private void yubiKey_Inserted()
        {
            setConfButton(true);
            setTestButton(true);
        }

        private void yubiKey_Removed()
        {
            setConfButton(false);
            setTestButton(false);
        }


        private void toggleEnabled(object sender, EventArgs e)
        {
            String username = userSelect.SelectedItem.ToString();
            RegistryKey userKey = Registry.LocalMachine.CreateSubKey(USERS + username);
            if (enableCheckBox.Checked == true)
            {
                userKey.SetValue("enabled", 1);
            }
            else
            {
                userKey.SetValue("enabled", 0);
            }
        }

        private void configureButton_Click(object sender, EventArgs e)
        {
            String username = userSelect.SelectedItem.ToString();
            byte[] salt = ConfigureNewSalt(username);
            if (ConfigureNewChallengeAndResponse(username, salt))
            {
                MessageBox.Show("Successfully configured YubiKey authentication",
                    "Success",
                    MessageBoxButtons.OK,
                    MessageBoxIcon.Information);
            }
            else
            {
                MessageBox.Show("Failed challenge-response",
                    "Failed",
                    MessageBoxButtons.OK,
                    MessageBoxIcon.Error);
            }
        }

        private byte[] ConfigureNewSalt(String username)
        {
            RegistryKey userKey = Registry.LocalMachine.CreateSubKey(USERS + username);
            RNGCryptoServiceProvider random = new RNGCryptoServiceProvider();
            byte[] salt = new byte[63];
            random.GetBytes(salt);
            userKey.SetValue("salt", salt.ToArray<byte>());
            return salt;
        }

        private bool ConfigureNewChallengeAndResponse(String username, byte[] salt)
        {
            RegistryKey userKey = Registry.LocalMachine.CreateSubKey(USERS + username);
            RNGCryptoServiceProvider random = new RNGCryptoServiceProvider();
            byte[] challenge = new byte[63];
            random.GetBytes(challenge);

            byte[] response = DoChallengeResponse(challenge, salt);
            if (response != null)
            {
                userKey.SetValue("nextChallenge", challenge.ToArray<byte>());
                userKey.SetValue("nextResponse", response);
                return true;
            }
            return false;
        }

        private byte[] DoChallengeResponse(byte[] challenge, byte[] salt)
        {
            String chal = BitConverter.ToString(challenge);
            chal.Replace("-", "");
            byte[] res = null;
            api.dataEncoding = ycENCODING.ycENCODING_BYTE_ARRAY;
            api.dataBuffer = chal;
            ycRETCODE ret = api.get_hmacSha1(2, ycCALL_MODE.ycCALL_BLOCKING);
            if (ret == ycRETCODE.ycRETCODE_OK)
            {
                Stopwatch sw = Stopwatch.StartNew();
                RegistryKey settings = Registry.LocalMachine.OpenSubKey(SETTINGS);
                int iterations = DEFAULT_ITERATIONS;
                object iterationsObj = settings.GetValue("hashIterations");
                if (iterationsObj != null)
                {
                    iterations = (int)iterationsObj;
                }
                byte[] response = api.dataBuffer;
                Rfc2898DeriveBytes pbkdf2 = new Rfc2898DeriveBytes(response, salt, iterations);
                res = pbkdf2.GetBytes(128);
                sw.Stop();
                testOutputLabel.Text = "Result hashing took " + sw.ElapsedMilliseconds / 1000.0 + " seconds.";
            }
            return res;
        }

        private void testButton_Click(object sender, EventArgs e)
        {
            String username = userSelect.SelectedItem.ToString();
            RegistryKey userKey = Registry.LocalMachine.CreateSubKey(USERS + username);
            byte[] challenge = (byte[])userKey.GetValue("nextChallenge");
            byte[] salt = (byte[])userKey.GetValue("salt");
            if (challenge == null || salt == null)
            {
                MessageBox.Show("Configure the key before running test",
                    "No Key configured",
                    MessageBoxButtons.OK,
                    MessageBoxIcon.Error);
                return;
            }
            
            byte[] response = DoChallengeResponse(challenge, salt);
            if (response == null)
            {
                MessageBox.Show("Failed Challenge-response");
                return;
            }
            byte[] correctResponse = (byte[])userKey.GetValue("nextResponse");
            if(Enumerable.SequenceEqual(response, correctResponse))
            {
                if (ConfigureNewChallengeAndResponse(username, salt))
                {
                    MessageBox.Show("Correct response!", "Test OK", MessageBoxButtons.OK, MessageBoxIcon.Information);
                }
                else
                {
                    MessageBox.Show("Test OK, new challenge and response failed.",
                        "Failed",
                        MessageBoxButtons.OK,
                        MessageBoxIcon.Error);
                }
            }
            else
            {
                MessageBox.Show("Wrong response!", "Test Not OK", MessageBoxButtons.OK, MessageBoxIcon.Error);
            }
        }

        private void setConfButton(bool enabled)
        {
            if (confButton.InvokeRequired)
            {
                setEnabledCallback d = new setEnabledCallback(setConfButton);
                this.Invoke(d, new object[] { enabled });
            }
            else
            {
                this.confButton.Enabled = enabled;
            }
        }

        private void setTestButton(bool enabled)
        {
            if (confButton.InvokeRequired)
            {
                setEnabledCallback d = new setEnabledCallback(setTestButton);
                this.Invoke(d, new object[] { enabled });
            }
            else
            {
                this.testButton.Enabled = enabled;
            }
        }

        private void setIterations_Click(object sender, EventArgs e)
        {
            if (iterationsInput.Text == null || iterationsInput.Text == "")
            {
                MessageBox.Show("You must enter a number of iterations",
                    "No iterations entered",
                    MessageBoxButtons.OK,
                    MessageBoxIcon.Error);
            }
            else
            {
                try
                {
                    int iterations = Convert.ToInt32(iterationsInput.Text);
                }
                catch (FormatException)
                {
                    MessageBox.Show("You must enter a number",
                        "Not a number",
                        MessageBoxButtons.OK,
                        MessageBoxIcon.Error);
                    return;
                }
                catch (OverflowException)
                {
                    MessageBox.Show("To large number entered",
                        "To large",
                        MessageBoxButtons.OK,
                        MessageBoxIcon.Error);
                    return;
                }
                RegistryKey settings = Registry.LocalMachine.CreateSubKey(SETTINGS);
                settings.SetValue("hashIterations", iterationsInput.Text, RegistryValueKind.DWord);
                MessageBox.Show("New iterations set, all users need to be reconfigured with the new iteration.",
                    "New iterations set",
                    MessageBoxButtons.OK,
                    MessageBoxIcon.Asterisk);
            }
        }

        private void iterations_KeyDown(object sender, KeyEventArgs e)
        {
            if (e.KeyCode == Keys.Enter)
            {
                setIterations_Click(sender, e);
            }
        }
    }
}