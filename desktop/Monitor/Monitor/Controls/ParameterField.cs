using System;
using System.ComponentModel;
using System.Windows.Forms;

namespace Monitor.Controls
{
    public partial class ParameterField : UserControl
    {
        public Data FieldData { get; set; }
        public ParameterGroup GroupParent { get; set; }

        public ParameterField(Data data, ParameterGroup groupParent)
        {
            InitializeComponent();
            FieldData = data;
            FieldData.Container = this;
            GroupParent = groupParent;

            lbl_Name.DataBindings.Add(new Binding("Text", FieldData, nameof(FieldData.Name), true));
            UpdateControls();
        }

        public void UpdateControls()
        {
            this.toolTip1.SetToolTip(this, FieldData.Tooltip);
            tb_String.DataBindings.Clear();

            if (FieldData.Type == Data.FieldType.Text)
            {
                tb_String.Enabled = true;
                tb_String.Visible = true;

                tb_Numeric.Enabled = false;
                tb_Numeric.Visible = false;

                cb_Boolean.Enabled = false;
                cb_Boolean.Visible = false;

                tb_String.DataBindings.Add(new Binding("Text", FieldData, nameof(FieldData.Value), true));
            }
            else if (FieldData.Type == Data.FieldType.Numeric)
            {
                tb_String.Enabled = false;
                tb_String.Visible = false;

                tb_Numeric.Enabled = true;
                tb_Numeric.Visible = true;

                cb_Boolean.Enabled = false;
                cb_Boolean.Visible = false;

                tb_Numeric.DataBindings.Add(new Binding("Text", FieldData, nameof(FieldData.Value), true));
            }
            else if (FieldData.Type == Data.FieldType.Boolean)
            {
                tb_String.Enabled = false;
                tb_String.Visible = false;

                tb_Numeric.Enabled = false;
                tb_Numeric.Visible = false;

                cb_Boolean.Enabled = true;
                cb_Boolean.Visible = true;

                cb_Boolean.DataBindings.Add(new Binding("Checked", FieldData, nameof(FieldData.Value), true));
            }

        }


        private void btn_Remove_Click(object sender, EventArgs e)
        {
            GroupParent.FieldsControls.Remove(this);
            GroupParent.GroupData.Fields.Remove(FieldData);
        }

        [Serializable]
        public class Data
        {

            public enum FieldType
            {
                [Description("Numerica")]
                Numeric,
                [Description("Texto")]
                Text,
                [Description("Booleana")]
                Boolean,
            }

            public ParameterField Container { get; internal set; }
            public ParameterGroup.Data ParentGroupData { get; internal set; }
            public string Name { get; set; }
            public string Tooltip { get; set; }

            public object Value { get; set; }


            private FieldType _type;

            public FieldType Type
            {
                get { return _type; }
                set {
                    if (value != _type)
                    {
                        _type = value;
                        Container.UpdateControls();
                    }
                }
            }


            public Data(ParameterGroup.Data groupData)
            {
                ParentGroupData = groupData;
            }
        }
    }
}
