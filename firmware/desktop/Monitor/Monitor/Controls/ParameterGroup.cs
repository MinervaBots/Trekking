using Monitor.Forms;
using System;
using System.Collections.Generic;
using System.Windows.Forms;
using System.Xml.Serialization;

namespace Monitor.Controls
{
    public partial class ParameterGroup : UserControl
    {
        public Data GroupData { get; set; }
        public ControlCollection FieldsControls { get { return gb_GroupContainer.Controls; } }

        public ParameterGroup(Data data)
        {
            InitializeComponent();

            GroupData = data;
            GroupData.Container = this;
            gb_GroupContainer.DataBindings.Add(new Binding("Text", GroupData, nameof(GroupData.Name), true));
        }

        private void btn_Remove_Click(object sender, EventArgs e)
        {
            GroupData.WasRemoved = true;
        }

        private void btn_NewField_Click(object sender, EventArgs e)
        {
            var fieldData = new ParameterField.Data(GroupData);
            using (var form = new EditFieldForm(fieldData, true))
            {
                if (form.ShowDialog() == DialogResult.OK)
                {
                    var fieldControl = new ParameterField(fieldData, this);
                    GroupData.Fields.Add(fieldData);
                    fieldControl.Location = new System.Drawing.Point(1, 17 + 30 * gb_GroupContainer.Controls.Count);
                    gb_GroupContainer.Controls.Add(fieldControl);
                    fieldControl.Anchor = (AnchorStyles.Top | AnchorStyles.Left | AnchorStyles.Right);
                }
            }
        }

        [Serializable]
        public class Data
        {
            public string Name { get; set; }
            public List<ParameterField.Data> Fields { get; set; } = new List<ParameterField.Data>();

            [XmlIgnore]
            public ParameterGroup Container { get; internal set; }
            [XmlIgnore]
            public bool WasRemoved { get; internal set; }
        }
    }
}
