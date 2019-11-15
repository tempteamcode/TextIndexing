import { BrowserModule } from '@angular/platform-browser';
import { NgModule } from '@angular/core';
import { FormsModule } from "@angular/forms"


import { AppComponent } from './app.component';
import { DocumentComponent } from './pages/document/document.component';
import { InvertedFileComponent } from './pages/inverted-file/inverted-file.component';
import { PageNotFoundComponent } from './pages/page-not-found/page-not-found.component';

import { RouterModule, Routes } from '@angular/router';
import { ResearchComponent } from './pages/research/research.component';

const appRoutes = [
  { path: 'research', component: ResearchComponent },
  { path: 'document', component: DocumentComponent },
  { path: 'inverted-file', component: InvertedFileComponent },
  { path: '', redirectTo: 'research', pathMatch: 'full' },
  { path: '**', component: PageNotFoundComponent }
];


@NgModule({
  declarations: [
    AppComponent,
    DocumentComponent,
    InvertedFileComponent,
    PageNotFoundComponent,
    ResearchComponent
  ],
  imports: [
    BrowserModule,
    FormsModule,
    RouterModule.forRoot(
      appRoutes,
      { enableTracing: false }
    )
  ],
  providers: [],
  bootstrap: [AppComponent]
})
export class AppModule { }
