using Monitor.Controls;
using System;
using System.ComponentModel;
using System.Linq;
using System.Windows.Forms;

namespace Monitor.Forms
{
    public partial class EditFieldForm : Form
    {
        private ParameterField.Data _fieldToEdit;

        public EditFieldForm(ParameterField.Data fieldToEdit, bool creating = false)
        {
            InitializeComponent();

            this.cb_ValueType.DataSource = Enum.GetValues(typeof(ParameterField.Data.FieldType))
                .Cast<Enum>()
                .Select(value => new
                {
                    (Attribute.GetCustomAttribute(value.GetType().GetField(value.ToString()), typeof(DescriptionAttribute)) as DescriptionAttribute).Description,
                    value
                })
                .OrderBy(item => item.value)
                .ToList();
            cb_ValueType.DisplayMember = "Description";
            cb_ValueType.ValueMember = "value";

            _fieldToEdit = fieldToEdit;
            if (creating)
            {
                Text = "Novo campo";
            }
            else
            {
                Text = "Editando campo";
            }

        
            groupBox1.Text = fieldToEdit.ParentGroupData.Name;
            Text += " - " + fieldToEdit.ParentGroupData.Name;
            
            tb_Name.Text = fieldToEdit.Name;
            tb_Tooltip.Text = fieldToEdit.Tooltip;
            cb_ValueType.DataBindings.Add(new Binding("SelectedValue", fieldToEdit, nameof(fieldToEdit.Type), true));
            tb_Name.DataBindings.Add(new Binding("Text", fieldToEdit, nameof(fieldToEdit.Name), true));
            tb_Tooltip.DataBindings.Add(new Binding("Text", fieldToEdit, nameof(fieldToEdit.Tooltip), true));

            timer1.Start();
        }

        private void button1_Click(object sender, EventArgs e)
        {
            switch (_fieldToEdit.Type)
            {
                case ParameterField.Data.FieldType.Text:
                    _fieldToEdit.Value = tb_DefaultValueText.Text;
                    break;
                case ParameterField.Data.FieldType.Numeric:
                    _fieldToEdit.Value = tb_DefaultValueNumeric.Text;
                    tb_DefaultValueNumeric.Visible = true;
                    break;
                case ParameterField.Data.FieldType.Boolean:
                    _fieldToEdit.Value = cb_DefaultValueBoolean.Checked;
                    break;
            }

            DialogResult = DialogResult.OK;
            Close();
        }

        private void timer1_Tick(object sender, EventArgs e)
        {
            if (_fieldToEdit == null)
                return;
            
            switch (_fieldToEdit.Type)
            {
                case ParameterField.Data.FieldType.Numeric:
                    if(!tb_DefaultValueNumeric.Visible)
                        tb_DefaultValueNumeric.Visible = true;
                    tb_DefaultValueText.Visible = false;
                    cb_DefaultValueBoolean.Visible = false;
                    break;

                case ParameterField.Data.FieldType.Text:
                    tb_DefaultValueNumeric.Visible = false;
                    if (!tb_DefaultValueText.Visible)
                        tb_DefaultValueText.Visible = true;
                    cb_DefaultValueBoolean.Visible = false;
                    break;

                case ParameterField.Data.FieldType.Boolean:
                    tb_DefaultValueNumeric.Visible = false;
                    tb_DefaultValueText.Visible = false;
                    if (!cb_DefaultValueBoolean.Visible)
                        cb_DefaultValueBoolean.Visible = true;
                    break;
            }
        }
    }
}
