export class Document {

  public date: Date = new Date()

  constructor( public id: string = "", public title: string = "", date: string = "", public contents: string[] = [] ) {
    this.date = new Date( `${date.substring(0,4)}-${date.substring(4,6)}-${date.substring(6,8)}` )
  }

}
