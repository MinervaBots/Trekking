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
            this.saveFileDialog1 = new System.Windows.Forms.SaveFileDialog();
            this.openFileDialog1 = new System.Windows.Forms.OpenFileDialog();
            this.gb_Variables = new System.Windows.Forms.GroupBox();
            this.btn_NewGroup = new System.Windows.Forms.Button();
            this.pnl_Variables = new System.Windows.Forms.Panel();
            this.gb_Actions = new System.Windows.Forms.GroupBox();
            this.timer1 = new System.Windows.Forms.Timer(this.components);
            this.ms_Main.SuspendLayout();
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
            this.gb_Variables.Controls.Add(this.btn_NewGroup);
            this.gb_Variables.Controls.Add(this.pnl_Variables);
            this.gb_Variables.Location = new System.Drawing.Point(13, 27);
            this.gb_Variables.Name = "gb_Variables";
            this.gb_Variables.Size = new System.Drawing.Size(277, 296);
            this.gb_Variables.TabIndex = 4;
            this.gb_Variables.TabStop = false;
            this.gb_Variables.Text = "Variáveis";
            // 
            // btn_NewGroup
            // 
            this.btn_NewGroup.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
            this.btn_NewGroup.Location = new System.Drawing.Point(253, -1);
            this.btn_NewGroup.Name = "btn_NewGroup";
            this.btn_NewGroup.Size = new System.Drawing.Size(25, 25);
            this.btn_NewGroup.TabIndex = 7;
            this.btn_NewGroup.Text = "+";
            this.btn_NewGroup.UseVisualStyleBackColor = true;
            this.btn_NewGroup.Click += new System.EventHandler(this.btn_NewGroup_Click);
            // 
            // pnl_Variables
            // 
            this.pnl_Variables.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
            | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.pnl_Variables.AutoScroll = true;
            this.pnl_Variables.Location = new System.Drawing.Point(10, 24);
            this.pnl_Variables.Name = "pnl_Variables";
            this.pnl_Variables.Size = new System.Drawing.Size(257, 261);
            this.pnl_Variables.TabIndex = 0;
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
            // timer1
            // 
            this.timer1.Tick += new System.EventHandler(this.timer1_Tick);
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
            this.gb_Variables.ResumeLayout(false);
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.MenuStrip ms_Main;

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
        private System.Windows.Forms.Button btn_NewGroup;
        private System.Windows.Forms.Timer timer1;
    }
}

