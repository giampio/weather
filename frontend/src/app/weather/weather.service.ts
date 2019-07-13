import { Injectable } from '@angular/core';
import { Weather } from '../classes/weather';
import { WeatherInterface } from '../interfaces/weather';
import { HttpClient } from '@angular/common/http';
import { Observable } from 'rxjs';
import { environment } from '../../environments/environment';

@Injectable()
export class WeatherService {
  weathers: Weather[] = [];
  private APIURL = environment.apiUrl + 'weather';

  constructor(private http: HttpClient) {

  }

  getWeathers(): Observable<any> {
    // return this.weathers;
    return this.http.get(this.APIURL);
  }

}
