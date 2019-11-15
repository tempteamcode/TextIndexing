import { BrowserModule } from '@angular/platform-browser';
import { NgModule } from '@angular/core';


import { AppComponent } from './app.component';
import { DocumentComponent } from './pages/document/document.component';
import { InvertedFileComponent } from './pages/inverted-file/inverted-file.component';
import { PageNotFoundComponent } from './pages/page-not-found/page-not-found.component';

import { RouterModule, Routes } from '@angular/router';

const appRoutes = [
  { path: 'document', component: DocumentComponent },
  { path: 'inverted-file', component: InvertedFileComponent },
  { path: '', redirectTo: 'document', pathMatch: 'full' },
  { path: '**', component: PageNotFoundComponent }
];


@NgModule({
  declarations: [
    AppComponent,
    DocumentComponent,
    InvertedFileComponent,
    PageNotFoundComponent
  ],
  imports: [
    BrowserModule,
    RouterModule.forRoot(
      appRoutes,
      { enableTracing: true }
    )
  ],
  providers: [],
  bootstrap: [AppComponent]
})
export class AppModule { }
