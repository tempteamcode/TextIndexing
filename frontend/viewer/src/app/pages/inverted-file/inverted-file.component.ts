import { Component, OnInit } from '@angular/core';
import { IfImporterService } from "../../services/ifimporter.service"

@Component({
  selector: 'app-inverted-file',
  templateUrl: './inverted-file.component.html',
  styleUrls: ['./inverted-file.component.css']
})
export class InvertedFileComponent implements OnInit {

  constructor( private ifPrvd: IfImporterService ) { }

  ngOnInit() {
  }

  regenerate() {
    this.ifPrvd.regenerateInvertedFile()
      .subscribe(
        res => {
          console.log( res )
        },
        err => {
          console.error( err )
        }
      )
  }

}
