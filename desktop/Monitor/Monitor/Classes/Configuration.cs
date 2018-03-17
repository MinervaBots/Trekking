using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Monitor.Classes
{
    [Serializable]
    public class Configuration
    {
        public string Name { get; set; }

        public List<ParameterGroup> Groups { get; set; } = new List<ParameterGroup>();
    }
}
