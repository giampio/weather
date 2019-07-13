import { BrowserModule } from '@angular/platform-browser';
import { NgModule } from '@angular/core';
import { FormsModule } from '@angular/forms';
import { AppComponent } from './app.component';
import { WeatherService } from './weather/weather.service';
import { WeatherComponent } from './weather/weather.component';
import { AngularFontAwesomeModule } from 'angular-font-awesome';
import { NgbModule} from '@ng-bootstrap/ng-bootstrap';
import { ChartsModule } from 'ng2-charts';
import { RouterModule, Routes } from '@angular/router';
import { HttpClientModule } from '@angular/common/http';

const routes: Routes = [
    {
      path: 'weather',
      component: WeatherComponent
    },    
    {
      path: '',
      redirectTo: '/weather',
      pathMatch: 'full'
    }
  ]

@NgModule({
  declarations: [
    AppComponent,   
    WeatherComponent
   
  ],
  imports: [
    BrowserModule,
    FormsModule,
    HttpClientModule,
    AngularFontAwesomeModule,
    NgbModule,
    RouterModule.forRoot(routes),
    ChartsModule
  ],
  providers: [WeatherService],
  bootstrap: [AppComponent]
})
export class AppModule { }
