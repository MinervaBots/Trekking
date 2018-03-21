using Monitor.Controls;
using System;
using System.Collections.Generic;

namespace Monitor.Classes
{
    [Serializable]
    public class Configuration
    {
        public string Name { get; set; }

        public List<ParameterGroup.Data> Groups { get; set; } = new List<ParameterGroup.Data>();
    }
}
