using System;

namespace Monitor
{
    partial class Main
    {
        /// <summary>
        /// Variável de designer necessária.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// Limpar os recursos que estão sendo usados.
        /// </summary>
        /// <param name="disposing">true se for necessário descartar os recursos gerenciados; caso contrário, false.</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Código gerado pelo Windows Form Designer

        /// <summary>
        /// Método necessário para suporte ao Designer - não modifique 
        /// o conteúdo deste método com o editor de código.
        /// </summary>
        private void InitializeComponent()
        {
            this.components = new System.ComponentModel.Container();
            this.ms_Main = new System.Windows.Forms.MenuStrip();
            this.arquivoToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.novoToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.abrirToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.salvarToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.monitorSerialToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.serialToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.logsToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.graficosToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.cms_Variables = new System.Windows.Forms.ContextMenuStrip(this.components);
            this.tsmi_NewGroup = new System.Windows.Forms.ToolStripMenuItem();
            this.cms_Group = new System.Windows.Forms.ContextMenuStrip(this.components);
            this.tsmi_NewField = new System.Windows.Forms.ToolStripMenuItem();
            this.cms_Field = new System.Windows.Forms.ContextMenuStrip(this.components);
            this.tsmi_EditField = new System.Windows.Forms.ToolStripMenuItem();
            this.tsmi_DeleteField = new System.Windows.Forms.ToolStripMenuItem();
            this.tsmi_CreateActionPacket = new System.Windows.Forms.ToolStripMenuItem();
            this.cms_Action = new System.Windows.Forms.ContextMenuStrip(this.components);
            this.saveFileDialog1 = new System.Windows.Forms.SaveFileDialog();
            this.openFileDialog1 = new System.Windows.Forms.OpenFileDialog();
            this.gb_Variables = new System.Windows.Forms.GroupBox();
            this.gb_Actions = new System.Windows.Forms.GroupBox();
            this.pnl_Variables = new System.Windows.Forms.Panel();
            this.ms_Main.SuspendLayout();
            this.cms_Variables.SuspendLayout();
            this.cms_Group.SuspendLayout();
            this.cms_Field.SuspendLayout();
            this.cms_Action.SuspendLayout();
            this.gb_Variables.SuspendLayout();
            this.SuspendLayout();
            // 
            // ms_Main
            // 
            this.ms_Main.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.arquivoToolStripMenuItem,
            this.monitorSerialToolStripMenuItem});
            this.ms_Main.Location = new System.Drawing.Point(0, 0);
            this.ms_Main.Name = "ms_Main";
            this.ms_Main.Size = new System.Drawing.Size(400, 24);
            this.ms_Main.TabIndex = 0;
            this.ms_Main.Text = "menuStrip1";
            // 
            // arquivoToolStripMenuItem
            // 
            this.arquivoToolStripMenuItem.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.novoToolStripMenuItem,
            this.abrirToolStripMenuItem,
            this.salvarToolStripMenuItem});
            this.arquivoToolStripMenuItem.Name = "arquivoToolStripMenuItem";
            this.arquivoToolStripMenuItem.Size = new System.Drawing.Size(61, 20);
            this.arquivoToolStripMenuItem.Text = "Arquivo";
            // 
            // novoToolStripMenuItem
            // 
            this.novoToolStripMenuItem.Name = "novoToolStripMenuItem";
            this.novoToolStripMenuItem.Size = new System.Drawing.Size(105, 22);
            this.novoToolStripMenuItem.Text = "Novo";
            this.novoToolStripMenuItem.Click += new System.EventHandler(this.novoToolStripMenuItem_Click);
            // 
            // abrirToolStripMenuItem
            // 
            this.abrirToolStripMenuItem.Name = "abrirToolStripMenuItem";
            this.abrirToolStripMenuItem.Size = new System.Drawing.Size(105, 22);
            this.abrirToolStripMenuItem.Text = "Abrir";
            this.abrirToolStripMenuItem.Click += new System.EventHandler(this.abrirToolStripMenuItem_Click);
            // 
            // salvarToolStripMenuItem
            // 
            this.salvarToolStripMenuItem.Name = "salvarToolStripMenuItem";
            this.salvarToolStripMenuItem.Size = new System.Drawing.Size(105, 22);
            this.salvarToolStripMenuItem.Text = "Salvar";
            this.salvarToolStripMenuItem.Click += new System.EventHandler(this.salvarToolStripMenuItem_Click);
            // 
            // monitorSerialToolStripMenuItem
            // 
            this.monitorSerialToolStripMenuItem.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.serialToolStripMenuItem,
            this.logsToolStripMenuItem,
            this.graficosToolStripMenuItem});
            this.monitorSerialToolStripMenuItem.Name = "monitorSerialToolStripMenuItem";
            this.monitorSerialToolStripMenuItem.Size = new System.Drawing.Size(62, 20);
            this.monitorSerialToolStripMenuItem.Text = "Monitor";
            // 
            // serialToolStripMenuItem
            // 
            this.serialToolStripMenuItem.Name = "serialToolStripMenuItem";
            this.serialToolStripMenuItem.Size = new System.Drawing.Size(117, 22);
            this.serialToolStripMenuItem.Text = "Serial";
            // 
            // logsToolStripMenuItem
            // 
            this.logsToolStripMenuItem.Name = "logsToolStripMenuItem";
            this.logsToolStripMenuItem.Size = new System.Drawing.Size(117, 22);
            this.logsToolStripMenuItem.Text = "Logs";
            // 
            // graficosToolStripMenuItem
            // 
            this.graficosToolStripMenuItem.Name = "graficosToolStripMenuItem";
            this.graficosToolStripMenuItem.Size = new System.Drawing.Size(117, 22);
            this.graficosToolStripMenuItem.Text = "Gráficos";
            // 
            // cms_Variables
            // 
            this.cms_Variables.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.tsmi_NewGroup});
            this.cms_Variables.Name = "cms_NewFull";
            this.cms_Variables.Size = new System.Drawing.Size(139, 26);
            // 
            // tsmi_NewGroup
            // 
            this.tsmi_NewGroup.Name = "tsmi_NewGroup";
            this.tsmi_NewGroup.Size = new System.Drawing.Size(138, 22);
            this.tsmi_NewGroup.Text = "Novo grupo";
            this.tsmi_NewGroup.Click += new System.EventHandler(this.tsmi_NewGroup_Click);
            // 
            // cms_Group
            // 
            this.cms_Group.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.tsmi_NewField});
            this.cms_Group.Name = "cms_NewField";
            this.cms_Group.Size = new System.Drawing.Size(144, 26);
            // 
            // tsmi_NewField
            // 
            this.tsmi_NewField.Name = "tsmi_NewField";
            this.tsmi_NewField.Size = new System.Drawing.Size(143, 22);
            this.tsmi_NewField.Text = "Novo campo";
            this.tsmi_NewField.Click += new System.EventHandler(this.tsmi_NewField_Click);
            // 
            // cms_Field
            // 
            this.cms_Field.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.tsmi_EditField,
            this.tsmi_DeleteField});
            this.cms_Field.Name = "cms_Field";
            this.cms_Field.Size = new System.Drawing.Size(152, 48);
            // 
            // tsmi_EditField
            // 
            this.tsmi_EditField.Name = "tsmi_EditField";
            this.tsmi_EditField.Size = new System.Drawing.Size(151, 22);
            this.tsmi_EditField.Text = "Editar campo";
            this.tsmi_EditField.Click += new System.EventHandler(this.tsmi_EditField_Click);
            // 
            // tsmi_DeleteField
            // 
            this.tsmi_DeleteField.Name = "tsmi_DeleteField";
            this.tsmi_DeleteField.Size = new System.Drawing.Size(151, 22);
            this.tsmi_DeleteField.Text = "Deletar campo";
            this.tsmi_DeleteField.Click += new System.EventHandler(this.tsmi_DeleteField_Click);
            // 
            // tsmi_CreateActionPacket
            // 
            this.tsmi_CreateActionPacket.Name = "tsmi_CreateActionPacket";
            this.tsmi_CreateActionPacket.Size = new System.Drawing.Size(138, 22);
            this.tsmi_CreateActionPacket.Text = "Criar pacote";
            this.tsmi_CreateActionPacket.Click += new System.EventHandler(this.tsmi_CreateActionPacket_Click);
            // 
            // cms_Action
            // 
            this.cms_Action.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.tsmi_CreateActionPacket});
            this.cms_Action.Name = "cms_Action";
            this.cms_Action.Size = new System.Drawing.Size(139, 26);
            // 
            // saveFileDialog1
            // 
            this.saveFileDialog1.FileName = "saveFileDialog1";
            this.saveFileDialog1.Filter = "XML file|*.xml";
            this.saveFileDialog1.Title = "Salve essa configuração em um arquivo";
            // 
            // openFileDialog1
            // 
            this.openFileDialog1.FileName = "openFileDialog1";
            this.openFileDialog1.Filter = "XML file|*.xml";
            this.openFileDialog1.Title = "Abra uma configuração salva em um arquivo";
            // 
            // gb_Variables
            // 
            this.gb_Variables.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
            | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.gb_Variables.ContextMenuStrip = this.cms_Variables;
            this.gb_Variables.Controls.Add(this.pnl_Variables);
            this.gb_Variables.Location = new System.Drawing.Point(13, 27);
            this.gb_Variables.Name = "gb_Variables";
            this.gb_Variables.Size = new System.Drawing.Size(277, 296);
            this.gb_Variables.TabIndex = 4;
            this.gb_Variables.TabStop = false;
            this.gb_Variables.Text = "Variáveis";
            // 
            // gb_Actions
            // 
            this.gb_Actions.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.gb_Actions.Location = new System.Drawing.Point(296, 27);
            this.gb_Actions.Name = "gb_Actions";
            this.gb_Actions.Size = new System.Drawing.Size(92, 296);
            this.gb_Actions.TabIndex = 5;
            this.gb_Actions.TabStop = false;
            this.gb_Actions.Text = "Ações";
            // 
            // pnl_Variables
            // 
            this.pnl_Variables.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
            | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.pnl_Variables.AutoScroll = true;
            this.pnl_Variables.Location = new System.Drawing.Point(10, 20);
            this.pnl_Variables.Name = "pnl_Variables";
            this.pnl_Variables.Size = new System.Drawing.Size(257, 260);
            this.pnl_Variables.TabIndex = 0;
            // 
            // Main
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(400, 335);
            this.Controls.Add(this.gb_Actions);
            this.Controls.Add(this.gb_Variables);
            this.Controls.Add(this.ms_Main);
            this.MainMenuStrip = this.ms_Main;
            this.MinimumSize = new System.Drawing.Size(300, 200);
            this.Name = "Main";
            this.Text = "MinervaBots";
            this.ms_Main.ResumeLayout(false);
            this.ms_Main.PerformLayout();
            this.cms_Variables.ResumeLayout(false);
            this.cms_Group.ResumeLayout(false);
            this.cms_Field.ResumeLayout(false);
            this.cms_Action.ResumeLayout(false);
            this.gb_Variables.ResumeLayout(false);
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.MenuStrip ms_Main;

        private System.Windows.Forms.ContextMenuStrip cms_Variables;
        private System.Windows.Forms.ToolStripMenuItem tsmi_NewGroup;

        private System.Windows.Forms.ContextMenuStrip cms_Group;
        private System.Windows.Forms.ToolStripMenuItem tsmi_NewField;

        private System.Windows.Forms.ContextMenuStrip cms_Field;
        private System.Windows.Forms.ToolStripMenuItem tsmi_EditField;
        private System.Windows.Forms.ToolStripMenuItem tsmi_DeleteField;

        private System.Windows.Forms.ContextMenuStrip cms_Action;
        private System.Windows.Forms.ToolStripMenuItem tsmi_CreateActionPacket;


        private System.Windows.Forms.ToolStripMenuItem arquivoToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem novoToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem abrirToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem salvarToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem monitorSerialToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem serialToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem logsToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem graficosToolStripMenuItem;
        private System.Windows.Forms.SaveFileDialog saveFileDialog1;
        private System.Windows.Forms.OpenFileDialog openFileDialog1;
        private System.Windows.Forms.GroupBox gb_Variables;
        private System.Windows.Forms.GroupBox gb_Actions;
        private System.Windows.Forms.Panel pnl_Variables;
    }
}

