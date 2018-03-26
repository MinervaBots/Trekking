namespace Monitor.Controls
{
    partial class ParameterField
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
            this.lbl_Name = new System.Windows.Forms.Label();
            this.toolTip1 = new System.Windows.Forms.ToolTip(this.components);
            this.tb_String = new System.Windows.Forms.TextBox();
            this.tb_Numeric = new System.Windows.Forms.TextBox();
            this.cb_Boolean = new System.Windows.Forms.CheckBox();
            this.btn_Edit = new System.Windows.Forms.Button();
            this.btn_Remove = new System.Windows.Forms.Button();
            this.SuspendLayout();
            // 
            // lbl_Name
            // 
            this.lbl_Name.AutoSize = true;
            this.lbl_Name.Location = new System.Drawing.Point(3, 9);
            this.lbl_Name.Name = "lbl_Name";
            this.lbl_Name.Size = new System.Drawing.Size(33, 13);
            this.lbl_Name.TabIndex = 0;
            this.lbl_Name.Text = "Label";
            // 
            // tb_String
            // 
            this.tb_String.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
            this.tb_String.Location = new System.Drawing.Point(94, 5);
            this.tb_String.Name = "tb_String";
            this.tb_String.Size = new System.Drawing.Size(77, 20);
            this.tb_String.TabIndex = 1;
            // 
            // tb_Numeric
            // 
            this.tb_Numeric.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
            this.tb_Numeric.Location = new System.Drawing.Point(94, 5);
            this.tb_Numeric.Name = "tb_Numeric";
            this.tb_Numeric.Size = new System.Drawing.Size(77, 20);
            this.tb_Numeric.TabIndex = 2;
            // 
            // cb_Boolean
            // 
            this.cb_Boolean.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
            this.cb_Boolean.AutoSize = true;
            this.cb_Boolean.Location = new System.Drawing.Point(152, 8);
            this.cb_Boolean.Name = "cb_Boolean";
            this.cb_Boolean.Size = new System.Drawing.Size(15, 14);
            this.cb_Boolean.TabIndex = 3;
            this.cb_Boolean.UseVisualStyleBackColor = true;
            // 
            // btn_Edit
            // 
            this.btn_Edit.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
            this.btn_Edit.Location = new System.Drawing.Point(178, 3);
            this.btn_Edit.Name = "btn_Edit";
            this.btn_Edit.Size = new System.Drawing.Size(25, 25);
            this.btn_Edit.TabIndex = 4;
            this.btn_Edit.Text = "e";
            this.btn_Edit.UseVisualStyleBackColor = true;
            // 
            // btn_Remove
            // 
            this.btn_Remove.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
            this.btn_Remove.Location = new System.Drawing.Point(206, 3);
            this.btn_Remove.Name = "btn_Remove";
            this.btn_Remove.Size = new System.Drawing.Size(25, 25);
            this.btn_Remove.TabIndex = 5;
            this.btn_Remove.Text = "x";
            this.btn_Remove.UseVisualStyleBackColor = true;
            this.btn_Remove.Click += new System.EventHandler(this.btn_Remove_Click);
            // 
            // ParameterField
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.Controls.Add(this.btn_Remove);
            this.Controls.Add(this.btn_Edit);
            this.Controls.Add(this.cb_Boolean);
            this.Controls.Add(this.tb_Numeric);
            this.Controls.Add(this.tb_String);
            this.Controls.Add(this.lbl_Name);
            this.Name = "ParameterField";
            this.Size = new System.Drawing.Size(230, 31);
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.Label lbl_Name;
        private System.Windows.Forms.ToolTip toolTip1;
        private System.Windows.Forms.TextBox tb_String;
        private System.Windows.Forms.TextBox tb_Numeric;
        private System.Windows.Forms.CheckBox cb_Boolean;
        private System.Windows.Forms.Button btn_Edit;
        private System.Windows.Forms.Button btn_Remove;
    }
}