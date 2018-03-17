using Monitor.Classes;
using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using Microsoft.VisualBasic;
using Monitor.Forms;
using System.Xml.Serialization;
using System.IO;
using System.Xml;

namespace Monitor
{
    public partial class Main : Form
    {
        private Configuration _configuration;

        public Main()
        {
            InitializeComponent();
            LoadConfiguration(Program.Configuration);
        }
        
        private void LoadConfiguration(Configuration config)
        {
            _configuration = config;
            foreach (var group in config.Groups)
            {
                CreateGroup(group);
            }
        }
        
        private void CreateGroup(ParameterGroup parameterGroup)
        {
            var groupBox = new GroupBox();
            groupBox.Text = parameterGroup.Name;
            groupBox.Width = pnl_Variables.Width;

            if (pnl_Variables.Controls.Count > 0)
            {
                var previousControl = pnl_Variables.Controls[pnl_Variables.Controls.Count - 1];
                var location = new Point(0, previousControl.Location.Y + previousControl.Height + 5);
                groupBox.Location = location;
            }

            groupBox.ContextMenuStrip = cms_Group;

            parameterGroup.Object = groupBox;
            groupBox.Height = 45;
            pnl_Variables.Controls.Add(groupBox);

            groupBox.Anchor = AnchorStyles.Left | AnchorStyles.Right | AnchorStyles.Top;
            foreach (var field in parameterGroup.Fields)
            {
                field.Parent = parameterGroup;
                CreateField(field, parameterGroup.Object);
            }
        }

        private void CreateField(ParameterField parameterField, Control parent)
        {
            Control field = null;
            switch (parameterField.Type)
            {
                case ParameterField.FieldType.Text:
                    field = new TextBox() { Text = parameterField.Value.ToString() };
                    break;
                case ParameterField.FieldType.Numeric:
                    field = new TextBox() { Text = parameterField.Value.ToString() };
                    break;
                case ParameterField.FieldType.Boolean:
                    field = new CheckBox() { Text = "", Checked = (bool)parameterField.Value };
                    break;
                    /*
                case ParameterField.FieldType.Selection:
                    field = new ComboBox();
                    break;
                    */
            }
            
            var label = new Label();
            label.Text = parameterField.Name;
            label.AutoSize = true;

            label.Location = new Point(10, 2);
            field.Location = new Point(140, 0);

            if (!string.IsNullOrWhiteSpace(parameterField.Tooltip))
            {
                var tooltip = new ToolTip();
                tooltip.SetToolTip(label, parameterField.Tooltip);
            }

            if(parameterField.Value != null)
                field.Text = parameterField.Value.ToString();

            field.DataBindings.Add("Text", parameterField, nameof(parameterField.Value));


            var panel = new Panel();
            panel.Location = new Point(10, 15 + 30 * parent.Controls.Count);
            panel.Anchor = AnchorStyles.Top | AnchorStyles.Right | AnchorStyles.Left;
            panel.ContextMenuStrip = cms_Field;
            panel.AutoSize = true;
            panel.Height = 20;

            panel.Controls.Add(label);
            panel.Controls.Add(field);
            
            parent.Controls.Add(panel);
            parent.Height = 15 + parent.Controls.Count * 30; 

            parameterField.Object = panel;
        }

        private void novoToolStripMenuItem_Click(object sender, EventArgs e)
        {
            pnl_Variables.Controls.Clear();
            Program.Configuration = new Configuration();
            LoadConfiguration(Program.Configuration);
        }

        private void tsmi_NewGroup_Click(object sender, EventArgs e)
        {
            var groupName = Interaction.InputBox("Digite o nome do novo grupo", "Novo grupo", "");
            if (string.IsNullOrWhiteSpace(groupName))
                return;

            var group = new ParameterGroup() { Name = groupName };

            _configuration.Groups.Add(group);
            CreateGroup(group);
        }

        private void tsmi_NewField_Click(object sender, EventArgs e)
        {
            var senderGroup = GetSenderGroup(sender);
            if (senderGroup == null)
            {
                return;
            }

            var parameterField = new ParameterField
            {
                Parent = senderGroup
            };

            var createFieldForm = new EditFieldForm(parameterField, true);
            if(createFieldForm.ShowDialog() == DialogResult.OK)
            {
                senderGroup.Fields.Add(parameterField);
                CreateField(parameterField, senderGroup.Object);
            }
        }

        private void salvarToolStripMenuItem_Click(object sender, EventArgs e)
        {
            if (saveFileDialog1.ShowDialog() != DialogResult.OK || string.IsNullOrWhiteSpace(saveFileDialog1.FileName))
                return;

            
            var serializer = new XmlSerializer(typeof(Configuration));

            using (StreamWriter writer = new StreamWriter(saveFileDialog1.FileName))
            {
                serializer.Serialize(writer, _configuration);
            }
        }

        private void abrirToolStripMenuItem_Click(object sender, EventArgs e)
        {
            if (openFileDialog1.ShowDialog() != DialogResult.OK || string.IsNullOrWhiteSpace(openFileDialog1.FileName))
                return;

            pnl_Variables.Controls.Clear();
            var serializer = new XmlSerializer(typeof(Configuration));

            using (StreamReader reader = new StreamReader(openFileDialog1.FileName))
            {
                if (serializer.Deserialize(reader) is Configuration configuration)
                {
                    Program.Configuration = configuration;
                    LoadConfiguration(configuration);
                }
            }
        }
        
        private void tsmi_DeleteField_Click(object sender, EventArgs e)
        {
            ParameterGroup group;
            var senderField = GetSenderField(sender, out group);

            if(group != null)
            {
                group.Fields.Remove(senderField);
                group.Object.Controls.Remove(senderField.Object as Control);
            }
        }

        private void tsmi_CreateActionPacket_Click(object sender, EventArgs e)
        {
            
        }

        private void tsmi_EditField_Click(object sender, EventArgs e)
        {
            ParameterGroup group;
            var senderField = GetSenderField(sender, out group);
            if (senderField == null)
            {
                return;
            }

            var editFieldForm = new EditFieldForm(senderField);
            if (editFieldForm.ShowDialog() == DialogResult.OK)
            {
                //var createdField = senderGroup.Fields[senderGroup.Fields.Count - 1];
                //CreateField(createdField, senderGroup.Object);
            }
        }


        private ParameterGroup GetSenderGroup(object sender)
        {
            ParameterGroup senderGroup = null;
            if (sender is ToolStripItem menuItem)
            {
                if (menuItem.Owner is ContextMenuStrip owner)
                {
                    foreach (var group in _configuration.Groups)
                    {
                        if (group.Object.Equals(owner.SourceControl))
                        {
                            senderGroup = group;
                            break;
                        }
                    }
                }
            }
            return senderGroup;
        }

        private object GetSenderField(object sender)
        {
            throw new NotImplementedException();
        }

        private ParameterField GetSenderField(object sender, out ParameterGroup group)
        {
            group = null;
            ParameterField  senderField = null;
            if (sender is ToolStripItem menuItem)
            {
                if (menuItem.Owner is ContextMenuStrip owner)
                {
                    foreach (var parameterGroup in _configuration.Groups)
                    {
                        foreach (var field in parameterGroup.Fields)
                        {
                            if (field.Object.Equals(owner.SourceControl))
                            {
                                group = parameterGroup;
                                senderField = field;
                                break;
                            }
                        }
                    }
                }
            }
            return senderField;
        }
    }
}
