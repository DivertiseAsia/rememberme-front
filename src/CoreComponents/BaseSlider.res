open React
open Belt

type slide = {
  content: React.element,
  slideNumber: int,
}

type sliderConfig = {
  maxSlidesToRender: int,
  loopSlides: bool,
  slideOffset: int,
}

let isSlideInRange = (slideNumber: int, currentSlideNumber: int, maxSlides: int, offset: int) =>
  slideNumber >= currentSlideNumber - offset &&
    slideNumber < currentSlideNumber + maxSlides - offset

let renderSlide = (slide: slide, offset: int) => {
  let className =
    switch offset {
    | 0 => "slide slide_current"
    | x => "slide slide_offset_" ++ string_of_int(x)
    } ++
    (" slide-" ++
    string_of_int(slide.slideNumber))
  <div className key={"slide-" ++ string_of_int(slide.slideNumber)}> slide.content </div>
}
@react.component
let make = (
  ~slides: list<slide>,
  ~currentSlide: int,
  ~config: sliderConfig,
  ~className: option<string>=?,
  ~children,
) => {
  let length = List.length(slides)
  let slideContent = List.map(slides, slide =>
    if (
      isSlideInRange(slide.slideNumber, currentSlide, config.maxSlidesToRender, config.slideOffset)
    ) {
      let offset = slide.slideNumber - currentSlide
      renderSlide(slide, offset)
    } else {
      React.null
    }
  )
  let beginExtra = switch (config.loopSlides, currentSlide < config.slideOffset) {
  | (true, true) =>
    List.map(slides, slide => {
      let offset = slide.slideNumber - length - currentSlide
      if offset >= -1 * config.slideOffset {
        renderSlide(slide, offset)
      } else {
        React.null
      }
    })
  | (_, _) => list{React.null}
  }
  let endExtra = switch (
    config.loopSlides,
    currentSlide > length - config.maxSlidesToRender + config.slideOffset,
  ) {
  | (true, true) =>
    List.map(slides, slide => {
      let offset = length - currentSlide + slide.slideNumber
      if offset <= config.slideOffset {
        renderSlide(slide, offset)
      } else {
        React.null
      }
    })
  | (_, _) => list{React.null}
  }

  <div className={"slider " ++ Js.Option.getWithDefault("slider-default", className)}>
    {array(List.toArray(beginExtra))}
    {array(List.toArray(slideContent))}
    {array(List.toArray(endExtra))}
    <div className="slider-other"> children </div>
  </div>
}
