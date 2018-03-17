using System;
using System.Collections.Generic;
using System.Windows.Forms;
using System.Xml.Serialization;

namespace Monitor
{
    [Serializable]
    public class ParameterGroup
    {
        public string Name { get; set; }
        public List<ParameterField> Fields { get; set; } = new List<ParameterField>();

        [XmlIgnore]
        public GroupBox Object { get; set; }
    }
}
