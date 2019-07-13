import { Component, OnInit, Output, EventEmitter } from '@angular/core';
import { Weather } from '../classes/weather';
import { WeatherService } from './weather.service';

@Component({
  selector: 'app-weather',
  templateUrl: './weather.component.html',
  styleUrls: ['./weather.component.css']
})
export class WeatherComponent implements OnInit {
  title = 'Weather';
  weathers: Weather[];

  constructor(private weatherService: WeatherService) { }

  chartOptions = {
    scaleShowVerticalLines: false,
    responsive: true
  };

  chartLabels: string[] = [];
  chartType = 'line';
  chartLegend = false;
  chartData: any[];

  ngOnInit() {

    this.weatherService.getWeathers().subscribe(
      response => {
        this.weathers = response;

        this.chartLabels = response.map(item => item.createdAt);
        this.chartData = [
          {data: response.map(item => item.temperature) },
        ];

        // console.log(this.chartData);
      },
      error => {
        console.log(error.message);
      }
    );

  }

}

