open BaseSlider
open Belt

type state = {slide: int}

type action =
  | NextSlide
  | GoToSlide(int)
  | PrevSlide

@react.component
let make = (
  ~initialSlide: int,
  ~slides: list<slide>,
  ~config: sliderConfig,
  ~className: option<string>=?,
) => {
  let (state, dispatch) = React.useReducer((state, action) =>
    switch action {
    // TODO: REWRITE
    | NextSlide =>
      if state.slide === List.length(slides) - 1 {
        config.loopSlides ? {slide: 0} : {slide: state.slide}
      } else {
        {
          slide: state.slide + 1,
        }
      }
    | PrevSlide =>
      if state.slide === 0 {
        config.loopSlides ? {slide: List.length(slides) - 1} : {slide: state.slide}
      } else {
        {
          slide: state.slide - 1,
        }
      }
    | GoToSlide(slide) => {slide: slide}
    }
  , {slide: initialSlide})
  <BaseSlider
    slides
    currentSlide=state.slide
    config
    className={"controlled-slider " ++
    Js.Option.getWithDefault("controlled-slider-default", className)}>
    {<>
      <div className="slider-controls">
        <div className="slide-left" onClick={_e => dispatch(PrevSlide)} />
        <div className="slide-right" onClick={_e => dispatch(NextSlide)} />
      </div>
      <div className="slider-bullets-container">
        <div className="slider-bullets">
          {React.array(
            List.toArray(
              List.mapWithIndex(slides, (index, slide) =>
                <div
                  key={"bullet-" ++ string_of_int(index)}
                  className={"slider-bullet" ++ (
                    slide.slideNumber === state.slide ? " slider-bullet-current" : ""
                  )}
                  onClick={_e => dispatch(GoToSlide(index))}
                />
              ),
            ),
          )}
        </div>
      </div>
    </>}
  </BaseSlider>
}
