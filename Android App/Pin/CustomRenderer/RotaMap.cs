using Newtonsoft.Json;
using System;
using System.Collections.Generic;
using System.Net;
using System.Net.Http;
using System.Text;
using System.Threading.Tasks;
using Xamarin.Forms;
using Xamarin.Forms.Maps;

namespace CustomRenderer
{
    public class RotaMap : ContentPage
    {
        DateTime dateTime;
        public RotaMap(DateTime dateTime)
        {
            this.dateTime = dateTime;
            ProcessDAtaAsync();
        }
        public async void ProcessDAtaAsync()
        {
            var customMap = new CustomMap
            {
                MapType = MapType.Street,
                WidthRequest = App.ScreenWidth,
                HeightRequest = App.ScreenHeight
            };
            List<KonumTablo> model = new List<KonumTablo>();
            model = await GetData();

            if (model.Count!=0)
            {
                foreach (var item in model)
                {
                    double latt = Convert.ToDouble(item.LatData) / 1000000;
                    double longg = Convert.ToDouble(item.LongData) / 1000000;
                    customMap.RouteCoordinates.Add(new Position(latt, longg));
                }
                customMap.MoveToRegion(MapSpan.FromCenterAndRadius(new Position(Convert.ToDouble(model[0].LatData) / 1000000, Convert.ToDouble(model[0].LongData) / 1000000), Distance.FromMiles(1.0)));
                Content = customMap;
            }
            else
            {
                await DisplayAlert("Hata", "Bu güne ait veri bulunamadı.", "Tamam");
                customMap.MoveToRegion(MapSpan.FromCenterAndRadius(new Position(39.196878, 33.175712), Distance.FromMiles(10.0)));
                Content = customMap;
            } 
        }

        public async Task<List<KonumTablo>> GetData()
        {
            List<KonumTablo> model = new List<KonumTablo>();
            string URL = "";

            var request = new HttpRequestMessage(HttpMethod.Get, URL + "/getAll/"+dateTime.Day.ToString() + "-"+dateTime.Month.ToString() + "-"+dateTime.Year.ToString());

            using (HttpClient client = new HttpClient())
            {
                var response = await client.SendAsync(request);

                if (response.StatusCode == HttpStatusCode.BadRequest)
                {
                    model = null;
                }

                else if (response.StatusCode == HttpStatusCode.OK)
                {
                    var content = await response.Content.ReadAsStringAsync();
                    model = JsonConvert.DeserializeObject<List<KonumTablo>>(content);
                    return model;
                }
                else
                {
                    model = null;
                    return model;
                }
                return model;
            }
        }
    }
}
