namespace Monitor.Controls
{
    partial class ParameterGroup
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
            this.gb_GroupContainer = new System.Windows.Forms.GroupBox();
            this.btn_NewField = new System.Windows.Forms.Button();
            this.btn_Remove = new System.Windows.Forms.Button();
            this.btn_Edit = new System.Windows.Forms.Button();
            this.SuspendLayout();
            // 
            // gb_GroupContainer
            // 
            this.gb_GroupContainer.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
            | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.gb_GroupContainer.Location = new System.Drawing.Point(12, 3);
            this.gb_GroupContainer.Name = "gb_GroupContainer";
            this.gb_GroupContainer.Size = new System.Drawing.Size(376, 215);
            this.gb_GroupContainer.TabIndex = 0;
            this.gb_GroupContainer.TabStop = false;
            this.gb_GroupContainer.Text = "Name";
            // 
            // btn_NewField
            // 
            this.btn_NewField.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
            this.btn_NewField.Location = new System.Drawing.Point(304, -1);
            this.btn_NewField.Name = "btn_NewField";
            this.btn_NewField.Size = new System.Drawing.Size(25, 25);
            this.btn_NewField.TabIndex = 8;
            this.btn_NewField.Text = "+";
            this.btn_NewField.UseVisualStyleBackColor = true;
            this.btn_NewField.Click += new System.EventHandler(this.btn_NewField_Click);
            // 
            // btn_Remove
            // 
            this.btn_Remove.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
            this.btn_Remove.Location = new System.Drawing.Point(364, -1);
            this.btn_Remove.Name = "btn_Remove";
            this.btn_Remove.Size = new System.Drawing.Size(25, 25);
            this.btn_Remove.TabIndex = 7;
            this.btn_Remove.Text = "x";
            this.btn_Remove.UseVisualStyleBackColor = true;
            this.btn_Remove.Click += new System.EventHandler(this.btn_Remove_Click);
            // 
            // btn_Edit
            // 
            this.btn_Edit.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
            this.btn_Edit.Location = new System.Drawing.Point(335, -1);
            this.btn_Edit.Name = "btn_Edit";
            this.btn_Edit.Size = new System.Drawing.Size(25, 25);
            this.btn_Edit.TabIndex = 6;
            this.btn_Edit.Text = "e";
            this.btn_Edit.UseVisualStyleBackColor = true;
            // 
            // ParameterGroup
            // 
            this.Controls.Add(this.btn_Remove);
            this.Controls.Add(this.btn_NewField);
            this.Controls.Add(this.btn_Edit);
            this.Controls.Add(this.gb_GroupContainer);
            this.Name = "ParameterGroup";
            this.Size = new System.Drawing.Size(400, 230);
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.GroupBox gb_GroupContainer;
        private System.Windows.Forms.Button btn_Remove;
        private System.Windows.Forms.Button btn_Edit;
        private System.Windows.Forms.Button btn_NewField;
    }
}