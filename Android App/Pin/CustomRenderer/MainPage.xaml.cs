using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

using Xamarin.Forms;
using Xamarin.Forms.Xaml;

namespace CustomRenderer
{
    [XamlCompilation(XamlCompilationOptions.Compile)]
    public partial class MainPage : ContentPage
    {
        DateTime datetimePulse;
        public MainPage()
        {
            
            InitializeComponent();
            datetimePulse = DateTime.Now;
        }
        private async void Rota_Clicked(object sender, EventArgs e)
        {
            await Navigation.PushModalAsync(new RotaMap(datetimePulse));
        }
        private void PulseDatepickerPulse_DateSelected(object sender, DateChangedEventArgs e)
        {
            datetimePulse = new DateTime(e.NewDate.Year, e.NewDate.Month, e.NewDate.Day);
        }

        private async void SonKonum_Clicked(object sender, EventArgs e)
        {
            await Navigation.PushModalAsync(new MapPage());
        }
    }
}