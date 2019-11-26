import { Injectable } from '@angular/core';

import { HttpClient, HttpHeaders } from '@angular/common/http';

@Injectable()
export class IfImporterService {

  constructor( private http: HttpClient ) { }

  getDocument( documentId: string = "" ) {

    let options: HttpHeaders = new HttpHeaders({ contentType: "text/plain" } )
    return this.http.get( "/api/document/" + documentId, { headers: options } )

  }

}
