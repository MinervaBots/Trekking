using System;
using System.ComponentModel;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Xml.Serialization;

namespace Monitor
{
    [Serializable]
    public class ParameterField
    {
        public enum FieldType
        {
            [Description("Numerica")]
            Numeric,
            [Description("Texto")]
            Text,
            [Description("Booleana")]
            Boolean,
            //[Description("Seleção")]
            //Selection
        }

        public string Name { get; set; }
        public string Tooltip { get; set; }
        public FieldType Type { get; set; }
        public object Value { get; set; }

        [XmlIgnore]
        public ParameterGroup Parent { get; set; }

        [XmlIgnore]
        public object Object { get; set; }
    }
}
