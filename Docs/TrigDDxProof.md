# $\frac{d}{dx} sin(x) = cos(x)$

$$
\begin{flalign}
\frac{d}{dx}sin(x) & = \lim_{h \to 0} \frac{sin(x + h) - sin(x)}{h} &\\
                   & = \lim_{h \to 0} \frac{sin(x) cos(h) + cos(x) sin(h) - sin(x)}{h} &\\
                   & = \lim_{h \to 0} \frac{cos(x) sin(h)}{h} + \lim_{h \to 0} \frac{sin(x) cos(h) - sin(x)}{h} &\\
                   & = cos(x) * \lim_{h \to 0} \frac{sin(h)}{h} + \lim_{h \to 0} \frac{sin(x) (cos(h) - 1)}{h} &\\
                   & = cos(x) * 1 + sin(x) * lim_{h \to 0} \frac{cos(h) - 1}{h} &\\
                   & = cos(x) + sin(x) * 0 &\\
                   & = \underline{\underline{cos(x)}}
\end{flalign}
$$

# $\frac{d}{dx} cos(x) = -sin(x)$

$$
\begin{flalign}
\frac{d}{dx}cos(x) & = \lim_{h \to 0} \frac{cos(x + h) - cos(x)}{h} &\\
                   & = \lim_{h \to 0} \frac{cos(x) cos(h) - sin(x) sin(h) - cos(x)}{h} &\\
                   & = \lim_{h \to 0} \frac{-sin(x) sin(h)}{h} + \lim_{h \to 0} \frac{cos(x) cos(h) - cos(x)}{h} &\\
                   & = -sin(x) * \lim_{h \to 0} \frac{sin(h)}{h} + \lim_{h \to 0} \frac{cos(x) (cos(h) - 1)}{h} &\\
                   & = -sin(x) * 1 + cos(x) * \lim_{h \to 0} \frac{cos(h) - 1}{h} &\\
                   & = -sin(x) + cos(x) * 0 &\\
                   & = \underline{\underline{-sin(x)}}
\end{flalign}
$$

# $\frac{d}{dx} tan(x) = sec^2(x)$

$$
\begin{flalign}
\frac{d}{dx}tan(x) & = \lim_{h \to 0} \frac{tan(x + h) - tan(x)}{h} &\\
                   & = \lim_{h \to 0} \frac{1}{h} \({\frac{sin(x + h)}{cos(x + h)} - \frac{sin(x)}{cos(x)}}\) &\\
                   & = \lim_{h \to 0} \frac{1}{h} \({\frac{sin(x + h) cos(x) - cos(x + h) sin(x)}{cos(x + h) cos(x)}}\) &\\
                   & = \lim_{h \to 0} \frac{1}{h} \({\frac{sin(x + h - x)}{cos(x + h) cos(x)}}\) &\\
                   & = \lim_{h \to 0} \frac{1}{h} \({\frac{sin(h)}{cos(x + h) cos(x)}}\) &\\
                   & = \lim_{h \to 0} \frac{sin(h)}{h} \({\frac{1}{cos(x) cos(x)}}\) &\\
                   & = \frac{1}{cos^2(x)} &\\
                   & = \underline{\underline{sec^2(x)}}
\end{flalign}
$$

# $\frac{d}{dx} sec(x) = sec(x) tan(x)$

$$
\begin{flalign}
\frac{d}{dx}sec(x) & = \lim_{h \to 0} \frac{1}{h} \({\frac{1}{cos(x + h)} - \frac{1}{cos(x)}}\) &\\
                   & = \lim_{h \to 0} \frac{1}{h} \({\frac{cos(x) - cos(x + h)}{cos(x + h) cos(x)}}\) &\\
                   & = \lim_{h \to 0} \frac{1}{h} \({\frac{2sin(\frac{h}{2}) sin(\frac{h}{2} + x)}{cos(x + h) cos(x)}}\) &\\
                   & = \lim_{h \to 0} \frac{sin(\frac{h}{2})}{\frac{h}{2}} \({\frac{2sin(\frac{h}{2} + x)}{2cos(x + h) cos(x)}}\) &\\
                   & = \frac{sin(x)}{cos(x) cos(x)} &\\
                   & = \frac{sin(x)}{cos^2(x)} &\\
                   & = \frac{1}{cos(x)} * \frac{sin(x)}{cos(x)} &\\
                   & = \underline{\underline{sec(x) tan(x)}}
\end{flalign}
$$
