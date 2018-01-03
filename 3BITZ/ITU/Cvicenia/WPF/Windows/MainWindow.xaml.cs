using System.Windows;
using System.Windows.Input;
using WPFWeather.ViewModel;

namespace WPFWeather.Windows
{
    public partial class MainWindow : Window
    {
        WeatherViewModel viewModel; 

        public MainWindow()
        {
            InitializeComponent();
            viewModel = new WeatherViewModel();
            viewModel.DownloadWeather("Brno");
            DataContext = viewModel;
        }

        private void changeCity(object sender, System.EventArgs e)
        {
            viewModel.DownloadWeather(city.Text);
        }
    }
}
