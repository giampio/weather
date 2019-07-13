import { WeatherInterface } from '../interfaces/weather';

export class Weather implements WeatherInterface {
  id: number;
  temperature: number;
  humidity: number;
  createdAt: string;
  updatedAt: string;
}
