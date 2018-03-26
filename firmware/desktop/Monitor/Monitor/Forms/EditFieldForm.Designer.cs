using System;
using System.ComponentModel;
using System.Linq;
using System.Windows.Forms;

namespace Monitor.Forms
{
    partial class EditFieldForm
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
            this.components = new System.ComponentModel.Container();
            this.button1 = new System.Windows.Forms.Button();
            this.groupBox1 = new System.Windows.Forms.GroupBox();
            this.cb_DefaultValueBoolean = new System.Windows.Forms.CheckBox();
            this.tb_DefaultValueNumeric = new System.Windows.Forms.TextBox();
            this.tb_DefaultValueText = new System.Windows.Forms.TextBox();
            this.label4 = new System.Windows.Forms.Label();
            this.cb_ValueType = new System.Windows.Forms.ComboBox();
            this.label3 = new System.Windows.Forms.Label();
            this.label2 = new System.Windows.Forms.Label();
            this.tb_Tooltip = new System.Windows.Forms.TextBox();
            this.label1 = new System.Windows.Forms.Label();
            this.tb_Name = new System.Windows.Forms.TextBox();
            this.timer1 = new System.Windows.Forms.Timer(this.components);
            this.groupBox1.SuspendLayout();
            this.SuspendLayout();
            // 
            // button1
            // 
            this.button1.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Right)));
            this.button1.Location = new System.Drawing.Point(197, 184);
            this.button1.Name = "button1";
            this.button1.Size = new System.Drawing.Size(75, 23);
            this.button1.TabIndex = 0;
            this.button1.Text = "Confirmar";
            this.button1.UseVisualStyleBackColor = true;
            this.button1.Click += new System.EventHandler(this.button1_Click);
            // 
            // groupBox1
            // 
            this.groupBox1.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
            | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.groupBox1.Controls.Add(this.cb_DefaultValueBoolean);
            this.groupBox1.Controls.Add(this.tb_DefaultValueNumeric);
            this.groupBox1.Controls.Add(this.tb_DefaultValueText);
            this.groupBox1.Controls.Add(this.label4);
            this.groupBox1.Controls.Add(this.cb_ValueType);
            this.groupBox1.Controls.Add(this.label3);
            this.groupBox1.Controls.Add(this.label2);
            this.groupBox1.Controls.Add(this.tb_Tooltip);
            this.groupBox1.Controls.Add(this.label1);
            this.groupBox1.Controls.Add(this.tb_Name);
            this.groupBox1.Location = new System.Drawing.Point(12, 12);
            this.groupBox1.Name = "groupBox1";
            this.groupBox1.Size = new System.Drawing.Size(260, 164);
            this.groupBox1.TabIndex = 1;
            this.groupBox1.TabStop = false;
            this.groupBox1.Text = "groupBox1";
            // 
            // cb_DefaultValueBoolean
            // 
            this.cb_DefaultValueBoolean.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
            this.cb_DefaultValueBoolean.AutoSize = true;
            this.cb_DefaultValueBoolean.Location = new System.Drawing.Point(160, 140);
            this.cb_DefaultValueBoolean.Name = "cb_DefaultValueBoolean";
            this.cb_DefaultValueBoolean.RightToLeft = System.Windows.Forms.RightToLeft.No;
            this.cb_DefaultValueBoolean.Size = new System.Drawing.Size(15, 14);
            this.cb_DefaultValueBoolean.TabIndex = 10;
            this.cb_DefaultValueBoolean.UseVisualStyleBackColor = true;
            this.cb_DefaultValueBoolean.Visible = false;
            // 
            // tb_DefaultValueNumeric
            // 
            this.tb_DefaultValueNumeric.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
            this.tb_DefaultValueNumeric.Location = new System.Drawing.Point(79, 137);
            this.tb_DefaultValueNumeric.Name = "tb_DefaultValueNumeric";
            this.tb_DefaultValueNumeric.Size = new System.Drawing.Size(163, 20);
            this.tb_DefaultValueNumeric.TabIndex = 9;
            this.tb_DefaultValueNumeric.Visible = false;
            // 
            // tb_DefaultValueText
            // 
            this.tb_DefaultValueText.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
            this.tb_DefaultValueText.Location = new System.Drawing.Point(79, 137);
            this.tb_DefaultValueText.Name = "tb_DefaultValueText";
            this.tb_DefaultValueText.Size = new System.Drawing.Size(163, 20);
            this.tb_DefaultValueText.TabIndex = 8;
            this.tb_DefaultValueText.Visible = false;
            // 
            // label4
            // 
            this.label4.AutoSize = true;
            this.label4.Location = new System.Drawing.Point(6, 140);
            this.label4.Name = "label4";
            this.label4.Size = new System.Drawing.Size(67, 13);
            this.label4.TabIndex = 7;
            this.label4.Text = "Valor padrão";
            // 
            // cb_ValueType
            // 
            this.cb_ValueType.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
            //this.cb_ValueType.FormattingEnabled = true;
            this.cb_ValueType.Location = new System.Drawing.Point(80, 102);
            this.cb_ValueType.Name = "cb_ValueType";
            this.cb_ValueType.Size = new System.Drawing.Size(163, 21);
            this.cb_ValueType.TabIndex = 6;
            this.cb_ValueType.DropDownStyle = ComboBoxStyle.DropDownList;
            // 
            // label3
            // 
            this.label3.AutoSize = true;
            this.label3.Location = new System.Drawing.Point(6, 105);
            this.label3.Name = "label3";
            this.label3.Size = new System.Drawing.Size(69, 13);
            this.label3.TabIndex = 5;
            this.label3.Text = "Tipo de valor";
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Location = new System.Drawing.Point(17, 70);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(39, 13);
            this.label2.TabIndex = 3;
            this.label2.Text = "Tooltip";
            // 
            // tb_Tooltip
            // 
            this.tb_Tooltip.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
            this.tb_Tooltip.Location = new System.Drawing.Point(80, 67);
            this.tb_Tooltip.Name = "tb_Tooltip";
            this.tb_Tooltip.Size = new System.Drawing.Size(163, 20);
            this.tb_Tooltip.TabIndex = 2;
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(17, 35);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(35, 13);
            this.label1.TabIndex = 1;
            this.label1.Text = "Nome";
            // 
            // tb_Name
            // 
            this.tb_Name.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
            this.tb_Name.Location = new System.Drawing.Point(80, 32);
            this.tb_Name.Name = "tb_Name";
            this.tb_Name.Size = new System.Drawing.Size(163, 20);
            this.tb_Name.TabIndex = 0;
            // 
            // timer1
            // 
            this.timer1.Tick += new System.EventHandler(this.timer1_Tick);
            // 
            // EditFieldForm
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(284, 217);
            this.Controls.Add(this.groupBox1);
            this.Controls.Add(this.button1);
            this.Name = "EditFieldForm";
            this.Text = "Novo campo";
            this.groupBox1.ResumeLayout(false);
            this.groupBox1.PerformLayout();
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.Button button1;
        private System.Windows.Forms.GroupBox groupBox1;
        private System.Windows.Forms.Label label4;
        private System.Windows.Forms.ComboBox cb_ValueType;
        private System.Windows.Forms.Label label3;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.TextBox tb_Tooltip;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.TextBox tb_Name;
        private System.Windows.Forms.TextBox tb_DefaultValueText;
        private System.Windows.Forms.TextBox tb_DefaultValueNumeric;
        private System.Windows.Forms.CheckBox cb_DefaultValueBoolean;
        private System.Windows.Forms.Timer timer1;
    }
}